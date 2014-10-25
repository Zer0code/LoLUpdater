// LoLUpdater.cpp : Defines the entry point for the console application.
//
#if defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 1300)

#include <immintrin.h>

int check_4th_gen_intel_core_features()
{
	const int the_4th_gen_features =
		(_FEATURE_AVX2 | _FEATURE_FMA | _FEATURE_BMI | _FEATURE_LZCNT | _FEATURE_MOVBE);
	return _may_i_use_cpu_feature(the_4th_gen_features);
}

#else /* non-Intel compiler */

#include <stdint.h>
#if defined(_MSC_VER)
# include <intrin.h>
#endif

void run_cpuid(uint32_t eax, uint32_t ecx, int* abcd)
{
#if defined(_MSC_VER)
	__cpuidex(abcd, eax, ecx);
#else
	uint32_t ebx, edx;
# if defined( __i386__ ) && defined ( __PIC__ )
	/* in case of PIC under 32-bit EBX cannot be clobbered */
	__asm__("movl %%ebx, %%edi \n\t cpuid \n\t xchgl %%ebx, %%edi" : "=D" (ebx),
# else
	__asm__("cpuid" : "+b" (ebx),
# endif
		"+a" (eax), "+c" (ecx), "=d" (edx));
	abcd[0] = eax; abcd[1] = ebx; abcd[2] = ecx; abcd[3] = edx;
#endif
}

int check_xcr0_ymm()
{
	uint32_t xcr0;
#if defined(_MSC_VER)
	xcr0 = (uint32_t)_xgetbv(0); /* min VS2010 SP1 compiler is required */
#else
	__asm__("xgetbv" : "=a" (xcr0) : "c" (0) : "%edx");
#endif
	return ((xcr0 & 6) == 6); /* checking if xmm and ymm state are enabled in XCR0 */
}


int check_4th_gen_intel_core_features()
{
	int abcd[4];
	uint32_t fma_movbe_osxsave_mask = ((1 << 12) | (1 << 22) | (1 << 27));
	uint32_t avx2_bmi12_mask = (1 << 5) | (1 << 3) | (1 << 8);

	/* CPUID.(EAX=01H, ECX=0H):ECX.FMA[bit 12]==1   &&
	CPUID.(EAX=01H, ECX=0H):ECX.MOVBE[bit 22]==1 &&
	CPUID.(EAX=01H, ECX=0H):ECX.OSXSAVE[bit 27]==1 */
	run_cpuid(1, 0, abcd);
	if ((abcd[2] & fma_movbe_osxsave_mask) != fma_movbe_osxsave_mask)
		return 0;

	if (!check_xcr0_ymm())
		return 0;

	/*  CPUID.(EAX=07H, ECX=0H):EBX.AVX2[bit 5]==1  &&
	CPUID.(EAX=07H, ECX=0H):EBX.BMI1[bit 3]==1  &&
	CPUID.(EAX=07H, ECX=0H):EBX.BMI2[bit 8]==1  */
	run_cpuid(7, 0, abcd);
	if ((abcd[1] & avx2_bmi12_mask) != avx2_bmi12_mask)
		return 0;

	/* CPUID.(EAX=80000001H):ECX.LZCNT[bit 5]==1 */
	run_cpuid(0x80000001, 0, abcd);
	if ((abcd[2] & (1 << 5)) == 0)
		return 0;

	return 1;
}

#endif /* non-Intel compiler */


static int can_use_intel_core_4th_gen_features()
{
	static int the_4th_gen_features_available = -1;
	/* test is performed once */
	if (the_4th_gen_features_available < 0)
		the_4th_gen_features_available = check_4th_gen_intel_core_features();

	return the_4th_gen_features_available;
}




#include <tchar.h>
#include <stdio.h>
#include "ShlObj.h"
#include <VersionHelpers.h>
#include "Urlmon.h"
#include "Windows.h"
#include "stdlib.h"
#include <string>
#include "Shlwapi.h"
#include <direct.h>

int _tmain(int argc, _TCHAR* argv[])
{

	char* buffer12 = nullptr;
	char* start = _getcwd(
		buffer12,
		200
		);
	char* cgbinpath = getenv("CG_BIN_PATH");
	if (cgbinpath == NULL)
	{
		URLDownloadToFileA(
			nullptr,
			"http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe",
			"Cg-3.1_April2012_Setup.exe",
			0,
			nullptr
			);
		char strcg[MAX_PATH];
		strcpy(strcg, start);
		strcat(strcg, "Cg-3.1_April2012_Setup.exe:Zone.Identifier");

		DeleteFileA(strcg);

		SHELLEXECUTEINFO ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = nullptr;
		ShExecInfo.lpVerb = nullptr;
		ShExecInfo.lpFile = L"Cg-3.1_April2012_Setup.exe";
		ShExecInfo.lpParameters = L"/verysilent /TYPE=compact";
		ShExecInfo.lpDirectory = nullptr;
		ShExecInfo.nShow = SW_SHOW;
		ShExecInfo.hInstApp = nullptr;
		ShellExecuteEx(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	}

	char str1[MAX_PATH];
	strcpy(str1, start);
	strcat(str1, "\\RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\Cg.dll");

	char str2[MAX_PATH];
	strcpy(str2, start);
	strcat(str2, "\\RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\CgGL.dll");

	char str3[MAX_PATH];
	strcpy(str3, start);
	strcat(str3, "\\RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\CgD3D9.dll");

	char str11[MAX_PATH];
	strcpy(str11, cgbinpath);
	strcat(str11, "\\Cg.dll");

	char str22[MAX_PATH];
	strcpy(str22, cgbinpath);
	strcat(str22, "\\CgGL.dll");

	char str33[MAX_PATH];
	strcpy(str33, cgbinpath);
	strcat(str33, "\\CgD3D9.dll");

	CopyFileA(
		str11,
		str1,
		false
		);
	CopyFileA(
		str22,
		str2,
		false
		);
	CopyFileA(
		str33,
		str3,
		false
		);
	char str0[MAX_PATH];
	strcpy(str0, start);
	strcat(str0, "\\RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\Cg.dll:Zone.Identifier");
	char str01[MAX_PATH];
	strcpy(str01, start);
	strcat(str01, "\\RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\CgGL.dll:Zone.Identifier");
	char str02[MAX_PATH];
	strcpy(str02, start);
	strcat(str02, "\\RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\CgD3D9.dll:Zone.Identifier");
	DeleteFileA(str0);
	DeleteFileA(str01);
	DeleteFileA(str02);


	char strair[MAX_PATH];
	strcpy(strair, start);
	strcat(strair, "air15_win.exe:Zone.Identifier");
	URLDownloadToFile(
		nullptr,
		L"https://labsdownload.adobe.com/pub/labs/flashruntimes/air/air15_win.exe",
		L"air15_win.exe",
		0,
		nullptr
		);
	DeleteFileA(strair);


	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = nullptr;
	ShExecInfo.lpVerb = nullptr;
	ShExecInfo.lpFile = L"air15_win.exe";
	ShExecInfo.lpParameters = L"-silent";
	ShExecInfo.lpDirectory = nullptr;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = nullptr;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

	//Todo: Convert this to TCHAR*
	char wide[MAX_PATH];


	SHGetFolderPathA(
		nullptr,
		CSIDL_PROGRAM_FILESX86,
		nullptr,
		NULL,
		wide
		);


	char str00[MAX_PATH];
	strcpy(str00, wide);
	strcat(str00, "\\Common Files\\Adobe AIR\\Versions\\1.0\\Adobe AIR.dll");
	char str000[MAX_PATH];
	strcpy(str000, start);
	strcat(str000, "\\RADS\\projects\\lol_air_client\\releases\\0.0.1.115\\deploy\\Adobe AIR\\Versions\\1.0\\Adobe AIR.dll");
	CopyFileA(
		str00,
		str000,
		false
		);

	char str001[MAX_PATH];
	strcpy(str001, wide);
	strcat(str001, "\\Common Files\\Adobe AIR\\Versions\\1.0\\Resources\\NPSWF32.dll");
	char str0001[MAX_PATH];
	strcpy(str0001, start);
	strcat(str0001, "\\RADS\\projects\\lol_air_client\\releases\\0.0.1.115\\deploy\\Adobe AIR\\Versions\\1.0\\Resources\\NPSWF32.dll");
	CopyFileA(
		str001,
		str0001,
		false
		);

	char str0000[MAX_PATH];
	strcpy(str0000, str000);
	strcat(str0000, ":Zone.Identifier");
	DeleteFileA(str0000);
	char str00001[MAX_PATH];
	strcpy(str00001, str0001);
	strcat(str00001, ":Zone.Identifier");
	DeleteFileA(str00001);

	char tbb[MAX_PATH];
	strcpy(tbb, start);
	strcat(tbb, "\\RADS\\solutions\\lol_game_client_sln\\releases\\0.0.1.62\\deploy\\tbb.dll");


#undef CONTEXT_XSTATE

#if defined(_M_X64)
#define CONTEXT_XSTATE                      (0x00100040)
#else
#define CONTEXT_XSTATE                      (0x00010040)
#endif


#define XSTATE_MASK_AVX                     (XSTATE_MASK_GSSE)

	typedef DWORD64(WINAPI *PGETENABLEDXSTATEFEATURES)();
	HMODULE hm = GetModuleHandle(_T("kernel32.dll"));

	PGETENABLEDXSTATEFEATURES pfnGetEnabledXStateFeatures = (PGETENABLEDXSTATEFEATURES)GetProcAddress(hm, "GetEnabledXStateFeatures");
	DWORD64 FeatureMask = pfnGetEnabledXStateFeatures();

	if (can_use_intel_core_4th_gen_features())
	{
	
		URLDownloadToFileA(
			nullptr,
			"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Avx2.dll",
			tbb,
			0,
			nullptr
			);

	}
	else
	{
		// AVX
		if (IsProcessorFeaturePresent(PF_XSAVE_ENABLED) & (FeatureMask & XSTATE_MASK_AVX) != 0)
		{
			URLDownloadToFileA(
				nullptr,
				"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Avx.dll",
				tbb,
				0,
				nullptr
				);
		}
		else
		{
			//SSE2
			if (IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE))
			{
				URLDownloadToFileA(
					nullptr,
					"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Sse2.dll",
					tbb,
					0,
					nullptr
					);
			}
			else
			{
				//SSE
				if (IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE))
				{
					URLDownloadToFileA(
						nullptr,
						"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Sse.dll",
						tbb,
						0,
						nullptr
						);
				}
				//Default
				else
				{
					URLDownloadToFileA(
						nullptr,
						"https://github.com/Loggan08/LoLUpdater/raw/master/Tbb/Default.dll",
						tbb,
						0,
						nullptr
						);
				}
			}
		}
	}
	char strtbb_c[MAX_PATH];
	strcpy(strtbb_c, tbb);
	strcat(strtbb_c, ":Zone.Identifier");
	DeleteFileA(strtbb_c);

	return 1;
}