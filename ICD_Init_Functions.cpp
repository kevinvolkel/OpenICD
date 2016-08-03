#include<string>
#include "ICD_sensing_BS_types.h" 
#include "ICD_Init_Functions.h"


//function to set NSR template structre
void NSR_Temp_Set(std::string EGM_num, struct4_T *icd_st) {
	
	if (EGM_num == "1") {
		icd_st->NSR_temp.refy[0] = 0.0213233080846678;	
		icd_st->NSR_temp.refy[1] = 0.0234677138934066;	
		icd_st->NSR_temp.refy[2] = 0.0187916012571564;
		icd_st->NSR_temp.refy[3] = -0.130124304654564;   
		icd_st->NSR_temp.refy[4] = 0.749136905185556;	
		icd_st->NSR_temp.refy[5] = 0.407976438400001;
		icd_st->NSR_temp.refy[6] = 0.841403411960980;	
		icd_st->NSR_temp.refy[7] = 0.707817267523610;
	}
	else if (EGM_num == "2") {
		icd_st->NSR_temp.refy[0] = 0.0220309361572486;
		icd_st->NSR_temp.refy[1] = 0.0311804135336094;
		icd_st->NSR_temp.refy[2] = 0.0263471037313190;
		icd_st->NSR_temp.refy[3] = -0.450261463863351;
		icd_st->NSR_temp.refy[4] = 0.578902702925651;
		icd_st->NSR_temp.refy[5] = 0.554907754281829;
		icd_st->NSR_temp.refy[6] = 0.339111349617473;
		icd_st->NSR_temp.refy[7] = 0.142461043418620;
	}
	else if (EGM_num == "3") {
		icd_st->NSR_temp.refy[0] = 0.0201098763767168;
		icd_st->NSR_temp.refy[1] = 0.0196799371969472;
		icd_st->NSR_temp.refy[2] = 0.0193390971588953;
		icd_st->NSR_temp.refy[3] = 0.0913083289194198;
		icd_st->NSR_temp.refy[4] = 0.810829145804936;
		icd_st->NSR_temp.refy[5] = -0.128757690829840;
		icd_st->NSR_temp.refy[6] = 0.134048075842370;
		icd_st->NSR_temp.refy[7] = 0.137446046885321;
	}
	else if (EGM_num == "4") {
		icd_st->NSR_temp.refy[0] = 0.0192524477593095;
		icd_st->NSR_temp.refy[1] = 0.0260075503153042;
		icd_st->NSR_temp.refy[2] = 0.0259429302551300;
		icd_st->NSR_temp.refy[3] = 0.380558978606759;
		icd_st->NSR_temp.refy[4] = 0.844289740398945;
		icd_st->NSR_temp.refy[5] = 0.814385231994032;
		icd_st->NSR_temp.refy[6] = 0.370537619231442;
		icd_st->NSR_temp.refy[7] = 0.106965471952829;
	}
	else if (EGM_num == "5") {
		icd_st->NSR_temp.refy[0] = 0.0236700468899885;
		icd_st->NSR_temp.refy[1] = 0.0204851636014723;
		icd_st->NSR_temp.refy[2] = 0.0157504242706746;
		icd_st->NSR_temp.refy[3] = -0.0344591163785369;
		icd_st->NSR_temp.refy[4] = 0.892174428689076;
		icd_st->NSR_temp.refy[5] = 0.372734034312465;
		icd_st->NSR_temp.refy[6] = 0.436210486467339;
		icd_st->NSR_temp.refy[7] = 0.229384034083781;
	}
	else if (EGM_num == "6") {
		icd_st->NSR_temp.refy[0] = 0.0260327963424746;
		icd_st->NSR_temp.refy[1] = 0.0277769663177891;
		icd_st->NSR_temp.refy[2] = 0.0237254940007960;
		icd_st->NSR_temp.refy[3] = -0.394377645741257;
		icd_st->NSR_temp.refy[4] = 0.191862051522425;
		icd_st->NSR_temp.refy[5] = 0.296324908740184;
		icd_st->NSR_temp.refy[6] = 0.188001539402085;
		icd_st->NSR_temp.refy[7] = 0.109929596204598;
	}
	else if (EGM_num == "7") {
		icd_st->NSR_temp.refy[0] = 0.0265928631253659;
		icd_st->NSR_temp.refy[1] = 0.0230674303837968;
		icd_st->NSR_temp.refy[2] = 0.0168649148913005;
		icd_st->NSR_temp.refy[3] = 0.0187667606294845;
		icd_st->NSR_temp.refy[4] = -0.599843894365305;
		icd_st->NSR_temp.refy[5] = 0.527899044283222;
		icd_st->NSR_temp.refy[6] = 0.282326933108916;
		icd_st->NSR_temp.refy[7] = 0.226280486633267;
	}
	else if (EGM_num == "8") {
		icd_st->NSR_temp.refy[0] = 0.0245332103760433;
		icd_st->NSR_temp.refy[1] = 0.0235815301680045;
		icd_st->NSR_temp.refy[2] = 0.0314441728765026;
		icd_st->NSR_temp.refy[3] = 0.0252485825840676;
		icd_st->NSR_temp.refy[4] = 0.0360936473399927;
		icd_st->NSR_temp.refy[5] = 0.828923722212234;
		icd_st->NSR_temp.refy[6] = -0.0276741075739319;
		icd_st->NSR_temp.refy[7] = -0.0247977734946870;
	}
	else if (EGM_num == "9") {
		icd_st->NSR_temp.refy[0] = 0.0300315812494885;
		icd_st->NSR_temp.refy[1] = 0.0259132985421315;
		icd_st->NSR_temp.refy[2] = 0.0349717566700220;
		icd_st->NSR_temp.refy[3] = -0.188512533379597;
		icd_st->NSR_temp.refy[4] = 0.769702871277252;
		icd_st->NSR_temp.refy[5] = 0.428237755538530;
		icd_st->NSR_temp.refy[6] = 0.831529297511848;
		icd_st->NSR_temp.refy[7] = 0.786800530238933;
	}

	else if (EGM_num == "10") {
		icd_st->NSR_temp.refy[0] = 0.0224294202528529;
		icd_st->NSR_temp.refy[1] = 0.0252260636639168;
		icd_st->NSR_temp.refy[2] = 0.0168361909505666;
		icd_st->NSR_temp.refy[3] = -0.340521169224960;
		icd_st->NSR_temp.refy[4] = 0.805073975199199;
		icd_st->NSR_temp.refy[5] = 0.822501490612861;
		icd_st->NSR_temp.refy[6] = 0.641550126644476;
		icd_st->NSR_temp.refy[7] = 0.534540752729187;
	}
	else if (EGM_num == "11") {
		icd_st->NSR_temp.refy[0] = 0.0253171658764302;
		icd_st->NSR_temp.refy[1] = 0.0236303513743403;
		icd_st->NSR_temp.refy[2] = 0.0243907092971680;
		icd_st->NSR_temp.refy[3] = 0.131414241963760;
		icd_st->NSR_temp.refy[4] = 0.299866831297742;
		icd_st->NSR_temp.refy[5] = 0.253545828675425;
		icd_st->NSR_temp.refy[6] = 0.149812018549771;
		icd_st->NSR_temp.refy[7] = 0.0438054139076148;
	}
	else if (EGM_num == "12") {
		icd_st->NSR_temp.refy[0] = 0.0221702296448063;
		icd_st->NSR_temp.refy[1] = 0.0222252982607956;
		icd_st->NSR_temp.refy[2] = 0.0264203061700565;
		icd_st->NSR_temp.refy[3] = -0.854180794021707;
		icd_st->NSR_temp.refy[4] = 0.680275786213115;
		icd_st->NSR_temp.refy[5] = 0.594525957469308;
		icd_st->NSR_temp.refy[6] = 0.437774033037062;
		icd_st->NSR_temp.refy[7] = 0.278399627129360;
	}
	else if (EGM_num == "13") {
		icd_st->NSR_temp.refy[0] = 0.0182061408702146;
		icd_st->NSR_temp.refy[1] = 0.0253888846074946;
		icd_st->NSR_temp.refy[2] = 0.0138657455801530;
		icd_st->NSR_temp.refy[3] = -0.224737355305665;
		icd_st->NSR_temp.refy[4] = 0.253232159269033;
		icd_st->NSR_temp.refy[5] = 0.274156365988914;
		icd_st->NSR_temp.refy[6] = 0.168244409724897;
		icd_st->NSR_temp.refy[7] = 0.0755511459378811;
	}
	else if (EGM_num == "14") {
		icd_st->NSR_temp.refy[0] = 0.0224715271433624;
		icd_st->NSR_temp.refy[1] = 0.0175239412506002;
		icd_st->NSR_temp.refy[2] = 0.0223325836542037;
		icd_st->NSR_temp.refy[3] = -0.329498471678327;
		icd_st->NSR_temp.refy[4] = 0.686344503333041;
		icd_st->NSR_temp.refy[5] = -0.560352964176186;
		icd_st->NSR_temp.refy[6] = -0.220103128692640;
		icd_st->NSR_temp.refy[7] = -0.183447303847287;
	}
	else if (EGM_num == "15") {
		icd_st->NSR_temp.refy[0] = 0.0181606459456032;
		icd_st->NSR_temp.refy[1] = 0.0220986263535992;
		icd_st->NSR_temp.refy[2] = 0.0161982195341597;
		icd_st->NSR_temp.refy[3] = -0.132222619091033;
		icd_st->NSR_temp.refy[4] = 0.477441492930813;
		icd_st->NSR_temp.refy[5] = 0.588201982699977;
		icd_st->NSR_temp.refy[6] = 0.454991001356387;
		icd_st->NSR_temp.refy[7] = 0.235827408033597;
	}
	else if (EGM_num == "16") {
		icd_st->NSR_temp.refy[0] = 0.0256200160593519;
		icd_st->NSR_temp.refy[1] = 0.0228837327010648;
		icd_st->NSR_temp.refy[2] = 0.0247644729747321;
		icd_st->NSR_temp.refy[3] = 0.0201626976250645;
		icd_st->NSR_temp.refy[4] = -0.871023658223053;
		icd_st->NSR_temp.refy[5] = 0.0787903692701740;
		icd_st->NSR_temp.refy[6] = 0.275556214712265;
		icd_st->NSR_temp.refy[7] = 0.296167645703808;
	}
	else if (EGM_num == "17") {
		icd_st->NSR_temp.refy[0] = 0.0256477029646124;
		icd_st->NSR_temp.refy[1] = 0.0263174202706379;
		icd_st->NSR_temp.refy[2] = 0.0213318950566051;
		icd_st->NSR_temp.refy[3] = -0.276042221164647;
		icd_st->NSR_temp.refy[4] = 0.627034960822915;
		icd_st->NSR_temp.refy[5] = 0.886868590006861;
		icd_st->NSR_temp.refy[6] = 0.787432811332760;
		icd_st->NSR_temp.refy[7] = -0.387021462867437;
	}
	else if (EGM_num == "18") {
		icd_st->NSR_temp.refy[0] = 0.0218205876348341;
		icd_st->NSR_temp.refy[1] = 0.0241127110775748;
		icd_st->NSR_temp.refy[2] = 0.0243952952683776;
		icd_st->NSR_temp.refy[3] = -0.373821926951331;
		icd_st->NSR_temp.refy[4] = 0.665227553236174;
		icd_st->NSR_temp.refy[5] = -0.669959741887857;
		icd_st->NSR_temp.refy[6] = -0.237446903664396;
		icd_st->NSR_temp.refy[7] = -0.173886593705329;
	}

	else if (EGM_num == "19") {
		icd_st->NSR_temp.refy[0] = 0.0258705459432105;
		icd_st->NSR_temp.refy[1] = 0.0296938901140683;
		icd_st->NSR_temp.refy[2] = 0.0252958662065717;
		icd_st->NSR_temp.refy[3] = 0.0227967684192681;
		icd_st->NSR_temp.refy[4] = 0.0207551557272808;
		icd_st->NSR_temp.refy[5] = 0.936776874031403;
		icd_st->NSR_temp.refy[6] = -0.0192057609166864;
		icd_st->NSR_temp.refy[7] = -0.0547212642783606;
	}
	else if (EGM_num == "22") {
		icd_st->NSR_temp.refy[0] = 0.0231087684976179;
		icd_st->NSR_temp.refy[1] = 0.0159964216326448;
		icd_st->NSR_temp.refy[2] = 0.0625524130375889;
		icd_st->NSR_temp.refy[3] = 0.446250465277758;
		icd_st->NSR_temp.refy[4] = -0.134898829181798;
		icd_st->NSR_temp.refy[5] = -0.0377723534280521;
		icd_st->NSR_temp.refy[6] = 0.00649055182099226;
		icd_st->NSR_temp.refy[7] = 0.0438419462055908;
	}













}