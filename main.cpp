#include <Engine.h>
#include<winsock2.h>
#include <iostream>
#include "ICD_sensing_init_BS.h"
#include  "ICD_sensing_BS.h"
#include "ICD_BS.h"
#include <vector>
#include <sstream>
#include <string>
#include <cmath>
#include <math.h>
#include <NIDAQmx.h>
#include <stdio.h>
#include <thread>
#include <chrono>
#include <ctime>
#include <process.h>   //reqd. for system function prototype    
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <Tchar.h>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include "dirent.h"
#include "SendFilestruct.h"
#include "Winsock_Functions.h"
#include<io.h>
#include<stdio.h>
#include <stdio.h>
#include<iomanip>



#pragma comment(lib,"ws2_32.lib") //Winsock Library
#pragma comment (lib, "libmat.lib")
#pragma comment (lib, "libmx.lib")
#pragma comment (lib, "libmex.lib")
#pragma comment (lib, "libeng.lib")

using namespace std;

#define EGM1 // #here 

int pausebutton(); // To hold on a button push 


struct0_T  icd_sense_st;
struct1_T  icd_sense_par;
struct2_T  icd_par;
struct3_T  NSR_template;
struct4_T  icd_st;


float64 input_data_buffer_A[100000] = { 0 };
float64 input_data_buffer_V[100000] = { 0 };
float64 input_data_buffer_S[100000] = { 0 };

int input_data_buffer_idx = 0;
int input_data_reading_idx = 0;

const int arraysize = 10000;
const  double degTorad = 0.0174;

double theraphy_sign = 0;
double inhibit_sign = 0;

double input_signals_sensing[2] = {0};

double V_input, A_input, A_BlanckP;
double vThresNom, vAGCNomThres, vRefPeriod, vAbsBlankPeriod,
vNoiseWindow, vFixedRefPeriod, vsAGCPeriod, vpAGCPeriod,
aThresNom, aAGCNomThres, aRefPeriod, aAbsBlankPeriod,
aNoiseWindow, aFixedRefPeriod, asAGCPeriod, apAGCPeriod, aCCBlockPeriod;

double initParams[17] = {0};

double input_shock_signal = 0;
char shock = 0;
long int print_idx = 0;
DIR *dir;
struct dirent *ent;
int file_idx = 1;

std::string filename;
std::ofstream writeOut;
std::ifstream inFile, inFile2;
std::string line, line2, field;

bool new_data_event = false;
bool data_written = false;

bool write_header = true;
bool header_written = false;


//Socket variables and strings for files
struct Filestruct struct4file;
WSADATA wsa;
SOCKET s;
struct sockaddr_in server;
char flag = 0;



//variables for socket receiving

int receivesize;
int log_file_num = 1;
char incoming[200000];


//DAQ handles and variables
int32       error = 0;
TaskHandle  taskHandle = 0;
TaskHandle  taskHandle2 = 0;


//Automation variables 
char start_ALG = 0;



//functions for the DAQ
#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else
int32 CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);
int32 CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);











//thread for the socket
int SOCKthread() {
	


	char socketerror;
	struct4file.linecount = 0;

	struct4file.counter = 0;

	struct4file.delimind;

	struct4file.bufpoint = struct4file.buffer;

	//initiate client
	socketerror = initclient(&wsa, &s, &server);
	if (socketerror == 1) return 0;

	//sending files to server, and receiving log files
	while (1) {
		//DAQmxStopTask(taskHandle);
		ifstream inFilemaster;


		//enter the name of the master file

		cout << "Enter the name of your file, enter done to close program ";

		getline(cin, struct4file.masterfile);

		cout << endl << endl;

		if (struct4file.masterfile != "done") {

			struct4file.masterfilename = struct4file.masterfile;

			inFilemaster.open(struct4file.masterfilename.c_str());

			if (!inFilemaster) {

				std::cout << "Unable to open file: " << struct4file.masterfilename << std::endl;

				return 0;

			}

			//loop for each line in the master file

			for (struct4file.line; std::getline(inFilemaster, struct4file.line);) {

				DAQmxStopTask(taskHandle);
				//get the EGM text file name
				std::istringstream in2(struct4file.line);

				in2 >> struct4file.textfilename;

				//function to send the EGM text file to PC2
				 send_EGM_txt(&s, &struct4file);

				//wait to move on to the next file
				cout << "waiting to send next file" << endl;
				
				//function that receives one log files from PC2
				log_file_num = rec_log(&s, log_file_num, incoming,&struct4file);
				//start the DAQ fir receiving
				DAQmxStartTask(taskHandle);
				//wait for final message from other PC to move on
				struct4file.receivesize = recv(s, struct4file.buffer, 1000, 0);
			}

		}
		// If someone types 'done'then close this program and tell other computer to close down
		else if (struct4file.masterfile == "done") {

			struct4file.message = "done";

			send(s, struct4file.message, strlen(struct4file.message), 0);

			//function to create test file
			create_test(log_file_num);

			flag = 1;
			break;
		}

	}

	return 0;



}







int DAQ1_thread() {


	float64     data[2] = { 0 };
	float64     data2[2] = { 0 };

	char        errBuff[2048] = { '\0' };
	char        errBuff2[2048] = { '\0' };

	icd_par.PVAB = 50;
	icd_par.PAVB = 40;
	icd_par.PVARP = 200;
	icd_par.VF_thresh = 320;
	icd_par.VT_thresh = 400;
	icd_par.zone_num = 2;
	icd_par.AFib_thresh = 200;
	icd_par.VFdur_length = 3000;
	icd_par.VTdur_length = 3000;

	icd_st.A_clk = 0;
	icd_st.V_clk = 0;
	icd_st.AS = 0;
	icd_st.AR = 0;
	icd_st.ABlk = 0;
	icd_st.VS = 0;
	icd_st.VT = 0;
	icd_st.VF = 0;
	icd_st.VBlk = 0;

	icd_st.VFduration = 0;
	icd_st.VFdur_count = 0;
	icd_st.VTduration = 0;
	icd_st.VTdur_count = 0;
	icd_st.VTC_ind = 1;

	for (int idx_st_VA = 0; idx_st_VA < 10; idx_st_VA++) {

		icd_st.V_win[idx_st_VA] = 1000000000;
		icd_st.A_win[idx_st_VA] = 1000000000;
		icd_st.VTC_win[idx_st_VA] = 1;
	}

	for (int idx_st_morph = 0; idx_st_morph < 200; idx_st_morph++) {
		icd_st.VTC_morph[idx_st_morph] = 0;
	}

	icd_st.NSR_temp.refx[0] = 1;
	icd_st.NSR_temp.refx[1] = 26;
	icd_st.NSR_temp.refx[2] = 51;
	icd_st.NSR_temp.refx[3] = 76;
	icd_st.NSR_temp.refx[4] = 101;
	icd_st.NSR_temp.refx[5] = 126;
	icd_st.NSR_temp.refx[6] = 151;
	icd_st.NSR_temp.refx[7] = 176;

	//EGM1-1
#ifdef EGM1
	icd_st.NSR_temp.refy[0] = 0.0213233080846678;	icd_st.NSR_temp.refy[1] = 0.0234677138934066;	icd_st.NSR_temp.refy[2] = 0.0187916012571564;
	icd_st.NSR_temp.refy[3] = -0.130124304654564;   icd_st.NSR_temp.refy[4] = 0.749136905185556;	icd_st.NSR_temp.refy[5] = 0.407976438400001;
	icd_st.NSR_temp.refy[6] = 0.841403411960980;	icd_st.NSR_temp.refy[7] = 0.707817267523610;
#endif

	//EGM1-2 
#ifdef EGM2
	icd_st.NSR_temp.refy[0] = 0.0220309361572486;	icd_st.NSR_temp.refy[1] = 0.0311804135336094;	icd_st.NSR_temp.refy[2] = 0.0263471037313190;
	icd_st.NSR_temp.refy[3] = -0.450261463863351;   icd_st.NSR_temp.refy[4] = 0.578902702925651;	icd_st.NSR_temp.refy[5] = 0.554907754281829;
	icd_st.NSR_temp.refy[6] = 0.339111349617473;	icd_st.NSR_temp.refy[7] = 0.142461043418620;
#endif
	//EGM1-3
#ifdef EGM3
	icd_st.NSR_temp.refy[0] = 0.0201098763767168;	icd_st.NSR_temp.refy[1] = 0.0196799371969472;	icd_st.NSR_temp.refy[2] = 0.0193390971588953;
	icd_st.NSR_temp.refy[3] = 0.0913083289194198;   icd_st.NSR_temp.refy[4] = 0.810829145804936;	icd_st.NSR_temp.refy[5] = -0.128757690829840;
	icd_st.NSR_temp.refy[6] = 0.134048075842370;	icd_st.NSR_temp.refy[7] = 0.137446046885321;
#endif
	//EGM1-4
#ifdef EGM4
	icd_st.NSR_temp.refy[0] = 0.0192524477593095;	icd_st.NSR_temp.refy[1] = 0.0260075503153042;	icd_st.NSR_temp.refy[2] = 0.0259429302551300;
	icd_st.NSR_temp.refy[3] = 0.380558978606759;    icd_st.NSR_temp.refy[4] = 0.844289740398945;	icd_st.NSR_temp.refy[5] = 0.814385231994032;
	icd_st.NSR_temp.refy[6] = 0.370537619231442;	icd_st.NSR_temp.refy[7] = 0.106965471952829;
#endif
	//EGM1-5
#ifdef EGM5
	icd_st.NSR_temp.refy[0] = 0.0236700468899885;	icd_st.NSR_temp.refy[1] = 0.0204851636014723;	icd_st.NSR_temp.refy[2] = 0.0157504242706746;
	icd_st.NSR_temp.refy[3] = -0.0344591163785369;   icd_st.NSR_temp.refy[4] = 0.892174428689076;	icd_st.NSR_temp.refy[5] = 0.372734034312465;
	icd_st.NSR_temp.refy[6] = 0.436210486467339;	icd_st.NSR_temp.refy[7] = 0.229384034083781;
#endif
	//EGM1-6
#ifdef EGM6
	icd_st.NSR_temp.refy[0] = 0.0260327963424746;	icd_st.NSR_temp.refy[1] = 0.0277769663177891;	icd_st.NSR_temp.refy[2] = 0.0237254940007960;
	icd_st.NSR_temp.refy[3] = -0.394377645741257;   icd_st.NSR_temp.refy[4] = 0.191862051522425;	icd_st.NSR_temp.refy[5] = 0.296324908740184;
	icd_st.NSR_temp.refy[6] = 0.188001539402085;	icd_st.NSR_temp.refy[7] = 0.109929596204598;
#endif
	//EGM1-7
#ifdef EGM7
	icd_st.NSR_temp.refy[0] = 0.0265928631253659;	icd_st.NSR_temp.refy[1] = 0.0230674303837968;	icd_st.NSR_temp.refy[2] = 0.0168649148913005;
	icd_st.NSR_temp.refy[3] = 0.0187667606294845;   icd_st.NSR_temp.refy[4] = -0.599843894365305;	icd_st.NSR_temp.refy[5] = 0.527899044283222;
	icd_st.NSR_temp.refy[6] = 0.282326933108916;	icd_st.NSR_temp.refy[7] = 0.226280486633267;
#endif
	//EGM1-8
#ifdef EGM8
	icd_st.NSR_temp.refy[0] = 0.0245332103760433;	icd_st.NSR_temp.refy[1] = 0.0235815301680045;	icd_st.NSR_temp.refy[2] = 0.0314441728765026;
	icd_st.NSR_temp.refy[3] = 0.0252485825840676;   icd_st.NSR_temp.refy[4] = 0.0360936473399927;	icd_st.NSR_temp.refy[5] = 0.828923722212234;
	icd_st.NSR_temp.refy[6] = -0.0276741075739319;	icd_st.NSR_temp.refy[7] = -0.0247977734946870;
#endif
	//EGM1-9
#ifdef EGM9
	icd_st.NSR_temp.refy[0] = 0.0300315812494885;	icd_st.NSR_temp.refy[1] = 0.0259132985421315;	icd_st.NSR_temp.refy[2] = 0.0349717566700220;
	icd_st.NSR_temp.refy[3] = -0.188512533379597;   icd_st.NSR_temp.refy[4] = 0.769702871277252;	icd_st.NSR_temp.refy[5] = 0.428237755538530;
	icd_st.NSR_temp.refy[6] = 0.831529297511848;	icd_st.NSR_temp.refy[7] = 0.786800530238933;
#endif
	//EGM1-10
#ifdef EGM10
	icd_st.NSR_temp.refy[0] = 0.0224294202528529;	icd_st.NSR_temp.refy[1] = 0.0252260636639168;	icd_st.NSR_temp.refy[2] = 0.0168361909505666;
	icd_st.NSR_temp.refy[3] = -0.340521169224960;   icd_st.NSR_temp.refy[4] = 0.805073975199199;	icd_st.NSR_temp.refy[5] = 0.822501490612861;
	icd_st.NSR_temp.refy[6] = 0.641550126644476;	icd_st.NSR_temp.refy[7] = 0.534540752729187;
#endif
	//EGM1-11
#ifdef EGM11
	icd_st.NSR_temp.refy[0] = 0.0253171658764302;	icd_st.NSR_temp.refy[1] = 0.0236303513743403;	icd_st.NSR_temp.refy[2] = 0.0243907092971680;
	icd_st.NSR_temp.refy[3] = 0.131414241963760;   icd_st.NSR_temp.refy[4] = 0.299866831297742;	icd_st.NSR_temp.refy[5] = 0.253545828675425;
	icd_st.NSR_temp.refy[6] = 0.149812018549771;	icd_st.NSR_temp.refy[7] = 0.0438054139076148;
#endif
	//EGM1-12
#ifdef EGM12
	icd_st.NSR_temp.refy[0] = 0.0221702296448063;	icd_st.NSR_temp.refy[1] = 0.0222252982607956;	icd_st.NSR_temp.refy[2] = 0.0264203061700565;
	icd_st.NSR_temp.refy[3] = -0.854180794021707;   icd_st.NSR_temp.refy[4] = 0.680275786213115;	icd_st.NSR_temp.refy[5] = 0.594525957469308;
	icd_st.NSR_temp.refy[6] = 0.437774033037062;	icd_st.NSR_temp.refy[7] = 0.278399627129360;
#endif
	//EGM1-13
#ifdef EGM13
	icd_st.NSR_temp.refy[0] = 0.0182061408702146;	icd_st.NSR_temp.refy[1] = 0.0253888846074946;	icd_st.NSR_temp.refy[2] = 0.0138657455801530;
	icd_st.NSR_temp.refy[3] = -0.224737355305665;   icd_st.NSR_temp.refy[4] = 0.253232159269033;	icd_st.NSR_temp.refy[5] = 0.274156365988914;
	icd_st.NSR_temp.refy[6] = 0.168244409724897;	icd_st.NSR_temp.refy[7] = 0.0755511459378811;
#endif
	//EGM1-14
#ifdef EGM14
	icd_st.NSR_temp.refy[0] = 0.0224715271433624;	icd_st.NSR_temp.refy[1] = 0.0175239412506002;	icd_st.NSR_temp.refy[2] = 0.0223325836542037;
	icd_st.NSR_temp.refy[3] = -0.329498471678327;   icd_st.NSR_temp.refy[4] = 0.686344503333041;	icd_st.NSR_temp.refy[5] = -0.560352964176186;
	icd_st.NSR_temp.refy[6] = -0.220103128692640;	icd_st.NSR_temp.refy[7] = -0.183447303847287;
#endif
	//EGM1-15
#ifdef EGM15
	icd_st.NSR_temp.refy[0] = 0.0181606459456032;	icd_st.NSR_temp.refy[1] = 0.0220986263535992;	icd_st.NSR_temp.refy[2] = 0.0161982195341597;
	icd_st.NSR_temp.refy[3] = -0.132222619091033;   icd_st.NSR_temp.refy[4] = 0.477441492930813;	icd_st.NSR_temp.refy[5] = 0.588201982699977;
	icd_st.NSR_temp.refy[6] = 0.454991001356387;	icd_st.NSR_temp.refy[7] = 0.235827408033597;
#endif
	//EGM1-16
#ifdef EGM16
	icd_st.NSR_temp.refy[0] = 0.0256200160593519;	icd_st.NSR_temp.refy[1] = 0.0228837327010648;	icd_st.NSR_temp.refy[2] = 0.0247644729747321;
	icd_st.NSR_temp.refy[3] = 0.0201626976250645;   icd_st.NSR_temp.refy[4] = -0.871023658223053;	icd_st.NSR_temp.refy[5] = 0.0787903692701740;
	icd_st.NSR_temp.refy[6] = 0.275556214712265;	icd_st.NSR_temp.refy[7] = 0.296167645703808;
#endif
	//EGM1-17
#ifdef EGM17
	icd_st.NSR_temp.refy[0] = 0.0256477029646124;	icd_st.NSR_temp.refy[1] = 0.0263174202706379;	icd_st.NSR_temp.refy[2] = 0.0213318950566051;
	icd_st.NSR_temp.refy[3] = -0.276042221164647;   icd_st.NSR_temp.refy[4] = 0.627034960822915;	icd_st.NSR_temp.refy[5] = 0.886868590006861;
	icd_st.NSR_temp.refy[6] = 0.787432811332760;	icd_st.NSR_temp.refy[7] = -0.387021462867437;
#endif
	//EGM1-18
#ifdef EGM18
	icd_st.NSR_temp.refy[0] = 0.0218205876348341;	icd_st.NSR_temp.refy[1] = 0.0241127110775748;	icd_st.NSR_temp.refy[2] = 0.0243952952683776;
	icd_st.NSR_temp.refy[3] = -0.373821926951331;   icd_st.NSR_temp.refy[4] = 0.665227553236174;	icd_st.NSR_temp.refy[5] = -0.669959741887857;
	icd_st.NSR_temp.refy[6] = -0.237446903664396;	icd_st.NSR_temp.refy[7] = -0.173886593705329;
#endif     
	//EGM1-19
#ifdef EGM19
	icd_st.NSR_temp.refy[0] = 0.0258705459432105;	icd_st.NSR_temp.refy[1] = 0.0296938901140683;	icd_st.NSR_temp.refy[2] = 0.0252958662065717;
	icd_st.NSR_temp.refy[3] = 0.0227967684192681;   icd_st.NSR_temp.refy[4] = 0.0207551557272808;	icd_st.NSR_temp.refy[5] = 0.936776874031403;
	icd_st.NSR_temp.refy[6] = -0.0192057609166864;	icd_st.NSR_temp.refy[7] = -0.0547212642783606;
#endif 

	//EGM1-22
#ifdef EGM22
	icd_st.NSR_temp.refy[0] = 0.0231087684976179;	icd_st.NSR_temp.refy[1] = 0.0159964216326448;	icd_st.NSR_temp.refy[2] = 0.0625524130375889;
	icd_st.NSR_temp.refy[3] = 0.446250465277758;   icd_st.NSR_temp.refy[4] = -0.134898829181798;	icd_st.NSR_temp.refy[5] = -0.0377723534280521;
	icd_st.NSR_temp.refy[6] = 0.00649055182099226;	icd_st.NSR_temp.refy[7] = 0.0438419462055908;
#endif 



	/*********************************************/
	// DAQmx Configure Code
	/*********************************************/
	

	//initiate the DAQ, sample once every 1ms
	DAQmxCreateTask("ACQUIRE", &taskHandle);
	DAQmxCreateAIVoltageChan(taskHandle, "Dev1/ai0", "", DAQmx_Val_RSE, -10.0, 10.0, DAQmx_Val_Volts, NULL);
	DAQmxCreateAIVoltageChan(taskHandle, "Dev1/ai1", "", DAQmx_Val_RSE, -10.0, 10.0, DAQmx_Val_Volts, NULL);
	DAQmxCreateAIVoltageChan(taskHandle, "Dev1/ai2", "", DAQmx_Val_RSE, -10.0, 10.0, DAQmx_Val_Volts, NULL);
	DAQmxCfgSampClkTiming(taskHandle, "", 1000.0, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 1);
	DAQmxRegisterEveryNSamplesEvent(taskHandle, DAQmx_Val_Acquired_Into_Buffer, 1, 0, EveryNCallback, NULL);
	DAQmxCreateAOVoltageChan(taskHandle, "Dev1/ao0", "", -10.0, 10.0, DAQmx_Val_Volts, "");
	DAQmxRegisterDoneEvent(taskHandle, 0, DoneCallback, NULL);

	/*********************************************/
	// DAQmx Start Code
	/*********************************************/

	

	while (header_written == false);
	write_header = false;

	while (flag==0) {
		while (new_data_event == false);
		// new data event is true here 

		// This part here is useful to verify acquisition
		//writeOut << input_data_reading_idx << '\t' << input_data_buffer_A[input_data_reading_idx] << '\t' << input_data_buffer_V[input_data_reading_idx] << '\t' << input_data_buffer_S[input_data_reading_idx] << endl;

	//	printf("r idx %i\t d1 %f\t d2 %f\t d3 %f \n", input_data_reading_idx, input_data_buffer_A[input_data_reading_idx], input_data_buffer_V[input_data_reading_idx], input_data_buffer_S[input_data_reading_idx]);
		input_signals_sensing[0] = input_data_buffer_V[input_data_reading_idx];
		input_signals_sensing[1] = input_data_buffer_A[input_data_reading_idx];
		input_shock_signal = input_data_buffer_S[input_data_reading_idx];
		input_data_reading_idx++;
		if (input_data_reading_idx == 100000) input_data_reading_idx = 0;



		ICD_sensing_BS(&icd_sense_st, &icd_sense_par, input_signals_sensing, &V_input, &A_input, &A_BlanckP); //Sensing
		ICD_BS(&icd_st, &icd_par, A_input, V_input, input_data_buffer_S[input_data_reading_idx], &theraphy_sign, &inhibit_sign); // Detection

		writeOut<<input_data_buffer_A[input_data_reading_idx] << endl;

		data_written = true;
	
	
	}


	return 0;
}

int main() {

	std::cout << "Hi\n!";
	int i = 0;

	if ((dir = opendir("../codegenTestFiles")) != NULL) {
		cout << "Succefully opened directory";
		while ((ent = readdir(dir)) != NULL) {
			printf("%s\n", ent->d_name);
		}
		cout << "Succefully opened directory, loading initialization file" << endl;
		//load initialization file
		filename = "../codegenTestFiles/codegen_sensing_test_BS";
		filename += ".init1"; // Load Parameters of Sensing for signal 1 #here
		cout << filename << endl;
		inFile.open(filename.c_str());
		if (!inFile) {
			cout << "Unable to open file: " << filename << endl;
			return 0;
		}

		double temp;
		std::getline(inFile, line);
		while (!inFile.eof()) {

			std::istringstream in(line);
			in >> field >> temp;
			initParams[i] = temp;
			cout << field << '\t' << initParams[i] << endl;
			i++;
			std::getline(inFile, line);
		}
		inFile.close();
		closedir(dir);

	}
	else {
		/* could not open directory */
		perror("");
		return 0;
	}

	ICD_sensing_init_BS(
		initParams[0],
		initParams[1],
		initParams[2],
		initParams[3],
		initParams[4],
		initParams[5],
		initParams[6],
		initParams[7],
		initParams[8],
		initParams[9],
		initParams[10],
		initParams[11],
		initParams[12],
		initParams[13],
		initParams[14],
		initParams[15],
		initParams[16],
		&icd_sense_st, &icd_sense_par);

	cout << "ICD sensing initalization complete" << endl<<endl; 
	

	std::thread t1(DAQ1_thread);
	std::thread t2(SOCKthread);

	t1.join();
	t2.join();


	while (flag==0);


	DAQmxStopTask(taskHandle);

	DAQmxClearTask(taskHandle);

	


	return 0;
}

int32 CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
{
	
	int32       error = 0;
	char        errBuff[2048] = { '\0' };
	static int  totalRead = 0;
	int32       read = 0;
	float64     puppa[1000];

	if (write_header == true) {
		filename = "output1.txt"; //#here
		

		writeOut.open(filename.c_str(), ios_base::out);
		writeOut<< std::fixed;
		writeOut << std::setprecision(6);
		//writeOut << "Idx  \t ARAW \t VRAW \t SRAW \t V input \t A input \t Theraphy \t A Inhibit \n";
		header_written = true;
	}

	/*********************************************/
	// DAQmx Read Code
	/*********************************************/
	DAQmxReadAnalogF64(taskHandle, 1, 10.0, DAQmx_Val_GroupByScanNumber, puppa, 3, &read, NULL);
	if (read > 0) {
		new_data_event =true;
		//Shift the voltages down to -1-1V
		puppa[0]--;
		puppa[1]--;
	    puppa[2]--;
		// To print data before being passes / Testintg 
		//printf("d1 %f\t d2 %f\t d3 %f\n", puppa[0], puppa[1], puppa[2]);
	   // writeOut << input_data_buffer_idx << "    " << puppa[0] <<"    " << puppa[1] << "    " << puppa[2] << '\n';

		input_data_buffer_A[input_data_buffer_idx] = puppa[0];  
		input_data_buffer_V[input_data_buffer_idx] = puppa[1];  
		input_data_buffer_S[input_data_buffer_idx] = puppa[2];  
		input_data_buffer_idx++;
		if (input_data_buffer_idx == 100000) input_data_buffer_idx = 0;
		if (puppa[0] > 1) shock = 0; 
		else shock = 1;

		while (data_written == false);
		new_data_event = false;

		return 1;
	}
	else return 0;
}

int32 CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData)
{
	int32   error = 0;
	char    errBuff[2048] = { '\0' };

Error:
	if (DAQmxFailed(error)) {
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		DAQmxClearTask(taskHandle);
		printf("DAQmx Error: %s\n", errBuff);
	}
	return 0;
}

int pausebutton(){
	cout << "Press a botton to continue... " << endl;
	while (!_kbhit());
	return(1);
}
