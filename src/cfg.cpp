﻿static char *cfg_id = 
	"@(#)Copyright (C) 2004-2016 H.Shirouzu		cfg.cpp	ver3.25";
/* ========================================================================
	Project  Name			: Fast/Force copy file and directory
	Create					: 2004-09-15(Wed)
	Update					: 2016-10-19(Wed)
	Copyright				: H.Shirouzu
	License					: GNU General Public License version 3
	Modify					: Mapaler 2015-08-23
	======================================================================== */

#include "mainwin.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>

#define FASTCOPY_INI_OLD		L"FastCopy.ini"
#define FASTCOPY_INI			L"FastCopy2.ini"
#define FASTCOPY_INI_OBSOLETE	L"FastCopy.ini-is-obsolete.txt"
#define FASTCOPY_OBSOLETE_MSG	"FastCopy.ini is obsolete, and FastCopy2.ini is used in FastCopy" \
								" ver3.0 or later.\r\n (FastCopy2.ini is encoded by UTF-8)\r\n"
#define MAIN_SECTION			"main"
#define INI_VERSION_KEY			"ini_version"
#define SRC_HISTORY				"src_history"
#define DST_HISTORY				"dst_history"
#define DEL_HISTORY				"del_history"
#define INC_HISTORY				"include_history"
#define EXC_HISTORY				"exclude_history"
#define FROMDATE_HISTORY		"fromdate_history"
#define TODATE_HISTORY			"todate_history"
#define MINSIZE_HISTORY			"minsize_history"
#define MAXSIZE_HISTORY			"maxsize_history"

#define MAX_HISTORY_KEY			"max_history"
#define COPYMODE_KEY			"default_copy_mode"
#define COPYFLAGS_KEY			"default_copy_flags"
#define COPYUNFLAGS_KEY			"default_copy_unflags"
#define SKIPEMPTYDIR_KEY		"skip_empty_dir"
#define FORCESTART_KEY			"force_start"
#define IGNORE_ERR_KEY			"ignore_error"
#define ESTIMATE_KEY			"estimate_mode"
#define DISKMODE_KEY			"disk_mode"
#define NETDRVMODE_KEY			"netdrv_mode"
#define ACLRESET_KEY			"acl_reset"
#define ISTOPLEVEL_KEY			"is_toplevel"
#define ISERRLOG_KEY			"is_errlog"
#define ISUTF8LOG_KEY			"is_utf8log"
#define FILELOGMODE_KEY			"filelog_mode"
#define FILELOGFLAGS_KEY		"filelog_flags"
#define ACLERRLOG_KEY			"aclerr_log"
#define STREAMERRLOG_KEY		"streamerr_log"
#define DEBUGFLAGS_KEY			"debug_flags"
#define DEBUGMAINFLAGS_KEY		"debug_main_flags"
#define ISRUNASBUTTON_KEY		"is_runas_button"
#define ISSAMEDIRRENAME_KEY		"is_samedir_rename"
#define BUFSIZE_KEY				"bufsize"
#define MAXTRANSSIZE_KEY		"max_transize"
#define MAXRUNNUM_KEY			"max_runnum"
#define MAXOVLSIZE_KEY			"max_ovlsize"
#define MAXOVLNUM_KEY			"max_ovlnum"
#define MAXOPENFILES_KEY		"max_openfiles"
#define MAXATTRSIZE_KEY			"max_attrsize"
#define MAXDIRSIZE_KEY			"max_dirsize"
#define MINSECTOR_KEY			"min_sectorsize"
//#define SHEXTAUTOCLOSE_KEY	"shext_autoclose"
//#define SHEXTTASKTRAY_KEY		"shext_tasktray"
//#define SHEXTNOCONFIRM_KEY	"shext_dd_noconfirm"
//#define SHEXTNOCONFIRMDEL_KEY	"shext_right_noconfirm"
#define EXECCONRIM_KEY			"exec_confirm"
#define FORCESTART_KEY			"force_start"
#define LCID_KEY				"lcid"
#define LOGFILE_KEY				"logfile"
#define WAITTICK_KEY			"wait_tick"
#define ISAUTOSLOWIO_KEY		"is_autoslow_io2"
#define SPEEDLEVEL_KEY			"speed_level"
#define ALWAYSLOWIO_KEY			"alwaysLowIo"
#define DRIVEMAP_KEY			"driveMap"
#define OWDEL_KEY				"overwrite_del"
#define ACL_KEY					"acl"
#define USEOVERLAPIO_KEY		"use_overlapio"
#define STREAM_KEY				"stream"
#define VERIFY_KEY				"verify"
#define USEMD5_KEY				"using_md5"
#define HASHMODE_KEY			"hash_mode"
#define NSA_KEY					"nsa_del"
#define DELDIR_KEY				"deldir_with_filter"
#define MOVEATTR_KEY			"move_attr"
#define SERIALMOVE_KEY			"serial_move"
#define SERIALVERIFYMOVE_KEY	"serial_verify_move"
#define REPARSE_KEY				"reparse2"
#define LINKDEST_KEY			"linkdest"
#define MAXLINKHASH_KEY			"max_linkhash"
#define ALLOWCONTFSIZE_KEY		"allow_cont_fsize"
#define RECREATE_KEY			"recreate"
#define EXTENDFILTER_KEY		"extend_filter"
#define WINPOS_KEY				"win_pos"
#define TASKBARMODE_KEY			"taskbarMode"
#define FINISHNOTIFY_KEY		"finish_notify"
#define FINISHNOTIFYTOUT_KEY	"finish_notify_tout"
#define INFOSPAN_KEY			"infoSpan"
#define STATUSFONT_KEY			"status_font"
#define STATUSFONTSIZE_KEY		"status_fontsize"
#define PREVENTSLEEP_KEY		"prevent_sleep"

#define NONBUFMINSIZENTFS_KEY	"nonbuf_minsize_ntfs2"
#define NONBUFMINSIZEFAT_KEY	"nonbuf_minsize_fat"
#define TIMEDIFFGRACE_KEY		"timediff_grace"
#define ISREADOSBUF_KEY			"is_readosbuf"
#define WRITESHAREOPEN_KEY		"writeshare_open"

#define FMT_JOB_KEY				"job_%d"
#define TITLE_KEY				"title"
#define CMD_KEY					"cmd"
#define SRC_KEY					"src"
#define DST_KEY					"dst"
#define FILTER_KEY				"filter"
#define INCLUDE_KEY				"include_filter"
#define EXCLUDE_KEY				"exclude_filter"
#define FROMDATE_KEY			"fromdate_filter"
#define TODATE_KEY				"todate_filter"
#define MINSIZE_KEY				"minsize_filter"
#define MAXSIZE_KEY				"maxsize_filter"
#define FMT_FINACT_KEY			"finaction_%d"
#define SOUND_KEY				"sound"
#define SHUTDOWNTIME_KEY		"shutdown_time"
#define FLAGS_KEY				"flags"

#define CUR_INI_VERSION			2
#define DEFAULT_MAX_HISTORY		10
#define DEFAULT_COPYMODE		1
#define DEFAULT_COPYFLAGS		0
#define DEFAULT_COPYUNFLAGS		0
#define DEFAULT_EMPTYDIR		1
#define DEFAULT_FORCESTART		0
#define DEFAULT_MAXRUNNUM		3
#define DEFAULT_MAXOVLNUM		4
#ifdef _WIN64
#define DEFAULT_BUFSIZE			128
#define DEFAULT_MAXTRANSSIZE	16
#define DEFAULT_MAXATTRSIZE		(1024 * 1024 * 1024)
#define DEFAULT_MAXDIRSIZE		(1024 * 1024 * 1024)
#else
#define DEFAULT_BUFSIZE			64
#define DEFAULT_MAXTRANSSIZE	8
#define DEFAULT_MAXATTRSIZE		(128 * 1024 * 1024)
#define DEFAULT_MAXDIRSIZE		(128 * 1024 * 1024)
#endif
#define DEFAULT_MAXOPENFILES	256
#define DEFAULT_NBMINSIZE_NTFS	64		// nbMinSize 参照
#define DEFAULT_NBMINSIZE_FAT	128		// nbMinSize 参照
#define DEFAULT_LINKHASH		300000
#define DEFAULT_ALLOWCONTFSIZE	(1024 * 1024 * 1024)
#define DEFAULT_WAITTICK		10
#define JOB_MAX					1000
#define FINACT_MAX				1000
#define DEFAULT_FASTCOPYLOG		"FastCopy.log"
#define DEFAULT_INFOSPAN		0 //默认更新速度等级

/*
	Vista以降
*/
#ifdef _WIN64
BOOL ConvertToX86Dir(WCHAR *target)
{
	WCHAR	buf[MAX_PATH];
	WCHAR	buf86[MAX_PATH];
	int		len;

	if (!::SHGetSpecialFolderPathW(NULL, buf, CSIDL_PROGRAM_FILES, FALSE)) return FALSE;
	len = (int)wcslen(buf);
	buf[len++] = '\\';
	buf[len] = 0;

	if (wcsnicmp(buf, target, len)) return FALSE;

	if (!::SHGetSpecialFolderPathW(NULL, buf86, CSIDL_PROGRAM_FILESX86, FALSE)) return FALSE;
	MakePathW(buf, buf86, target + len);
	wcscpy(target, buf);

	return	 TRUE;
}
#endif

BOOL ConvertVirtualStoreConf(WCHAR *execDir, WCHAR *userDir, WCHAR *virtualDir)
{
	WCHAR	buf[MAX_PATH];
	WCHAR	org_ini[MAX_PATH], usr_ini[MAX_PATH], vs_ini[MAX_PATH];
	BOOL	is_admin = ::IsUserAnAdmin();
	BOOL	is_exists;

	MakePathW(usr_ini, userDir, FASTCOPY_INI);
	MakePathW(org_ini, execDir, FASTCOPY_INI);

#ifdef _WIN64
	ConvertToX86Dir(org_ini);
#endif

	is_exists = ::GetFileAttributesW(usr_ini) != 0xffffffff;
	 if (!is_exists) {
		::CreateDirectoryW(userDir, NULL);
	}

	if (virtualDir && virtualDir[0]) {
		MakePathW(vs_ini,  virtualDir, FASTCOPY_INI);
		if (::GetFileAttributesW(vs_ini) != 0xffffffff) {
			if (!is_exists) {
				is_exists = ::CopyFileW(vs_ini, usr_ini, TRUE);
			}
			MakePathW(buf, userDir, L"to_OldDir(VirtualStore).lnk");
			SymLinkW(virtualDir, buf);
			swprintf(buf, L"%s.obsolete", vs_ini);
			::MoveFileW(vs_ini, buf);
			if (::GetFileAttributesW(vs_ini) != 0xffffffff) {
				::DeleteFileW(vs_ini);
			}
		}
	}

	if ((is_admin || !is_exists) && ::GetFileAttributesW(org_ini) != 0xffffffff) {
		if (!is_exists) {
			is_exists = ::CopyFileW(org_ini, usr_ini, TRUE);
		}
		if (is_admin) {
			swprintf(buf, L"%s.obsolete", org_ini);
			::MoveFileW(org_ini, buf);
			if (::GetFileAttributesW(org_ini) != 0xffffffff) {
				::DeleteFileW(org_ini);
			}
		}
	}

	MakePathW(buf, userDir, L"to_ExeDir.lnk");
	if (::GetFileAttributesW(buf) == 0xffffffff) {
		SymLinkW(execDir, buf);
	}

	return	TRUE;
}


/*=========================================================================
  クラス ： Cfg
  概  要 ： コンフィグクラス
  説  明 ： 
  注  意 ： 
=========================================================================*/
Cfg::Cfg()
{
}

Cfg::~Cfg()
{
	free(virtualDir);
	free(userDir);
	free(errLogPath);
	free(execDir);
	free(execPath);
}

BOOL Cfg::Init(WCHAR *user_dir, WCHAR *virtual_dir)
{
	WCHAR	buf[MAX_PATH], path[MAX_PATH], *fname = NULL;

	::GetModuleFileNameW(NULL, buf, MAX_PATH);
	::GetFullPathNameW(buf, MAX_PATH, path, &fname);
	if (!fname) {
		return FALSE;
	}

	execPath = wcsdup(path);
	fname[-1] = 0; // remove '\\' of dir\\fname
	execDir = wcsdup(path);

	errLogPath = NULL;
	userDir = NULL;
	virtualDir = NULL;

	if (IsWinVista() && TIsVirtualizedDirW(execDir)) {
		if (user_dir) {
			userDir = wcsdup(user_dir);
			if (virtual_dir) {
				virtualDir = wcsdup(virtual_dir);
			}
		}
		else {
			WCHAR	virtual_store[MAX_PATH];
			WCHAR	fastcopy_dir[MAX_PATH];
			WCHAR	*fastcopy_dirname = NULL;

			::GetFullPathNameW(path, MAX_PATH, fastcopy_dir, &fastcopy_dirname);

			::SHGetSpecialFolderPathW(NULL, buf, CSIDL_APPDATA, FALSE);
			MakePathW(path, buf, fastcopy_dirname);
			userDir = wcsdup(path);

			wcscpy(buf, execDir);
#ifdef _WIN64
			ConvertToX86Dir(buf);
#endif
			if (!TMakeVirtualStorePathW(buf, virtual_store)) {
				return FALSE;
			}
			virtualDir = wcsdup(virtual_store);
		}
		ConvertVirtualStoreConf(execDir, userDir, virtualDir);
	}
	if (!userDir) {
		userDir = wcsdup(execDir);
	}

	WCHAR	ini_path[MAX_PATH];
	MakePathW(ini_path, userDir, FASTCOPY_INI);

	ini.Init(ini_path);
	ini.SetSection(MAIN_SECTION);
	needIniConvert = FALSE;

	if (ini.GetInt(INI_VERSION_KEY, -1) < CUR_INI_VERSION) {
		if (::GetFileAttributesW(ini_path) != 0xffffffff) {
			WCHAR	bak_path[MAX_PATH];
			wcscpy(bak_path, ini_path);
			wcscat(bak_path, L".bak");
			if (!::MoveFileExW(ini_path, bak_path, MOVEFILE_REPLACE_EXISTING)) {
				::DeleteFileW(ini_path);
			}
			ini.UnInit();
			ini.Init(ini_path);
		}
		WCHAR	ini_path_old[MAX_PATH];
		MakePathW(ini_path_old, userDir, FASTCOPY_INI_OLD);

		if (::GetFileAttributesW(ini_path_old) != 0xffffffff) {
			ini.UnInit();
			ini.Init(ini_path_old); // loading done
			ini.SetIniFileNameW(ini_path);
			needIniConvert = TRUE;

			// obsolete message file
			MakePathW(ini_path_old, userDir, FASTCOPY_INI_OBSOLETE);
			HANDLE hFile = ::CreateFileW(ini_path_old, GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
											FILE_ATTRIBUTE_NORMAL, 0);
			DWORD	size;
			::WriteFile(hFile, FASTCOPY_OBSOLETE_MSG, sizeof(FASTCOPY_OBSOLETE_MSG), &size, 0);
			::CloseHandle(hFile);
		}
	}

	return	TRUE;
}

void ConvertFilter(const WCHAR *s, WCHAR *d)
{
	int		len = (int)wcslen(s);
	WCHAR	ch;
	int		si = 0;
	int		di = 0;
	BOOL	is_charclass = FALSE;

	while ((ch = s[si++])) {
		if (ch == '\\' && si < len && !is_charclass) {
			ch = s[si++];
			d[di++] = '[';
			d[di++] = '\\';
			d[di++] = ch;
			d[di++] = ']';
		}
		else {
			d[di++] = ch;
			switch (ch) {
			case '[':  if (!is_charclass) is_charclass = TRUE;
				break;
			case ']':  if (is_charclass)  is_charclass = FALSE;
				break;
			case '\\': if (si < len) d[di++] = s[si++];
				break;
			default:
				break;
			}
		}
	}
	d[di] = 0;

	DebugW(L"%s -> %s\n", s, d);
}

BOOL Cfg::GetFilterStr(const char *key, char *tmpbuf, WCHAR *wbuf)
{
	ini.GetStr(key, tmpbuf, MAX_HISTORY_CHAR_BUF, "");
	IniStrToW(tmpbuf, wbuf);

	if (needIniConvert) {
		PathArray pa;

		pa.RegisterMultiPath(wbuf);

		for (int i=0; i < pa.Num(); i++) {
			ConvertFilter(pa.Path(i), wbuf);
			pa.ReplacePath(i, wbuf);
		}
		pa.GetMultiPath(wbuf, MAX_HISTORY_CHAR_BUF);
	}

	return	TRUE;
}

//#include <crtdbg.h>
//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_CHECK_ALWAYS_DF|_CRTDBG_CHECK_CRT_DF|_CRTDBG_DELAY_FREE_MEM_DF);
//_ASSERTE( _CrtCheckMemory( ) );

BOOL Cfg::ReadIni(WCHAR *user_dir, WCHAR *virtual_dir)
{
	if (!Init(user_dir, virtual_dir)) return FALSE;

	int		i, j;
	char	section[100], key[100], *p;
	DynBuf	buf(MAX_HISTORY_CHAR_BUF);
	Wstr	wbuf(MAX_HISTORY_CHAR_BUF);
	char	*section_array[] = {	SRC_HISTORY, DST_HISTORY, DEL_HISTORY,
									INC_HISTORY, EXC_HISTORY,
									FROMDATE_HISTORY, TODATE_HISTORY,
									MINSIZE_HISTORY, MAXSIZE_HISTORY };
	bool	is_filter_array[] = {	false, false, false,
									true, true,
									false, false,
									false, false };
	WCHAR	***history_array[] = {	&srcPathHistory, &dstPathHistory, &delPathHistory,
									&includeHistory, &excludeHistory,
									&fromDateHistory, &toDateHistory,
									&minSizeHistory, &maxSizeHistory };

/*
	WCHAR	wtestbuf[MAX_HISTORY_CHAR_BUF];
	WCHAR	*wtest[] = {
							L"12345",
							L"abc\\[abc\\]\\",
							L"aaa\\aaa",
							L"abc[\\ax]",
							L"abc[\\ax]\\",
							L"12[[]345",
							L"12[[]345\\",
							L"12\\a3\\[45\\",
							L"12345",
							NULL };


	for (int i=0; wtest[i]; i++) GetFilterStrCore(wtest[i], wtestbuf);
*/
	srcPathHistory	= NULL;
	dstPathHistory	= NULL;
	delPathHistory	= NULL;
	includeHistory	= NULL;
	excludeHistory	= NULL;

	jobArray = NULL;
	jobMax = 0;

	finActArray = NULL;
	finActMax = 0;

	ini.SetSection(MAIN_SECTION);
	iniVersion		= ini.GetInt(INI_VERSION_KEY, CUR_INI_VERSION);
	bufSize			= ini.GetInt(BUFSIZE_KEY, DEFAULT_BUFSIZE);
	maxRunNum		= ini.GetInt(MAXRUNNUM_KEY, DEFAULT_MAXRUNNUM);
	maxTransSize	= ini.GetInt(MAXTRANSSIZE_KEY, DEFAULT_MAXTRANSSIZE);
	maxTransSize	= min(maxTransSize, 4095);
	maxOvlNum		= ini.GetInt(MAXOVLNUM_KEY, DEFAULT_MAXOVLNUM);
	maxOvlSize		= ini.GetInt(MAXOVLSIZE_KEY, -1);
	maxOvlSize		= min(maxOvlSize, 4095);

	if ((maxTransSize % maxOvlNum)) {
		maxTransSize = (maxTransSize + maxOvlNum - 1) / maxOvlNum * maxOvlNum;
	}
	if (bufSize < maxTransSize * BUFIO_SIZERATIO) {
		bufSize = maxTransSize * BUFIO_SIZERATIO;
	}
	maxOpenFiles	= ini.GetInt(MAXOPENFILES_KEY, DEFAULT_MAXOPENFILES);
	maxAttrSize		= ini.GetInt(MAXATTRSIZE_KEY, DEFAULT_MAXATTRSIZE);
#ifdef WIN64
	if (maxAttrSize < DEFAULT_MAXATTRSIZE) {	// 32bit ini ?
		maxAttrSize = DEFAULT_MAXATTRSIZE;
	}
#endif
	maxDirSize		= ini.GetInt(MAXDIRSIZE_KEY, DEFAULT_MAXDIRSIZE);
#ifdef WIN64
	if (maxDirSize < DEFAULT_MAXDIRSIZE) {	// 32bit ini ?
		maxDirSize = DEFAULT_MAXDIRSIZE;
	}
#endif
	minSectorSize	= ini.GetInt(MINSECTOR_KEY, 0);
	if (minSectorSize % 512) {
		minSectorSize = 0;
	}

	nbMinSizeNtfs	= ini.GetInt(NONBUFMINSIZENTFS_KEY, DEFAULT_NBMINSIZE_NTFS);
	nbMinSizeFat	= ini.GetInt(NONBUFMINSIZEFAT_KEY, DEFAULT_NBMINSIZE_FAT);
	timeDiffGrace	= ini.GetInt(TIMEDIFFGRACE_KEY, 0);

	isReadOsBuf		= ini.GetInt(ISREADOSBUF_KEY, FALSE);
	isWShareOpen	= ini.GetInt(WRITESHAREOPEN_KEY, FALSE);
	maxHistoryNext	= maxHistory = ini.GetInt(MAX_HISTORY_KEY, DEFAULT_MAX_HISTORY);
	copyMode		= ini.GetInt(COPYMODE_KEY, DEFAULT_COPYMODE);
	copyFlags		= ini.GetInt(COPYFLAGS_KEY, DEFAULT_COPYFLAGS);
	copyUnFlags		= ini.GetInt(COPYUNFLAGS_KEY, DEFAULT_COPYUNFLAGS);

	skipEmptyDir	= ini.GetInt(SKIPEMPTYDIR_KEY, DEFAULT_EMPTYDIR);
	forceStart		= ini.GetInt(FORCESTART_KEY, DEFAULT_FORCESTART);
	ignoreErr		= ini.GetInt(IGNORE_ERR_KEY, TRUE);
	estimateMode	= ini.GetInt(ESTIMATE_KEY, 0);
	diskMode		= ini.GetInt(DISKMODE_KEY, 0);
	netDrvMode		= ini.GetInt(NETDRVMODE_KEY, 0);
	aclReset		= ini.GetInt(ACLRESET_KEY, 0);
	isTopLevel		= ini.GetInt(ISTOPLEVEL_KEY, FALSE);
	isErrLog		= ini.GetInt(ISERRLOG_KEY, TRUE);
	isUtf8Log		= ini.GetInt(ISUTF8LOG_KEY, TRUE);
	fileLogMode		= ini.GetInt(FILELOGMODE_KEY, 0);
	fileLogFlags	= ini.GetInt(FILELOGFLAGS_KEY, 0);
	aclErrLog		= ini.GetInt(ACLERRLOG_KEY, FALSE);
	streamErrLog	= ini.GetInt(STREAMERRLOG_KEY, FALSE);
	debugFlags		= ini.GetInt(DEBUGFLAGS_KEY, 0);
	debugMainFlags	= ini.GetInt(DEBUGMAINFLAGS_KEY, 0);
	isRunasButton	= ini.GetInt(ISRUNASBUTTON_KEY, FALSE);
	isSameDirRename	= ini.GetInt(ISSAMEDIRRENAME_KEY, TRUE);

//	shextAutoClose	= ini.GetInt(SHEXTAUTOCLOSE_KEY, TRUE);
//	shextTaskTray	= ini.GetInt(SHEXTTASKTRAY_KEY, FALSE);
//	shextNoConfirm	= ini.GetInt(SHEXTNOCONFIRM_KEY, FALSE);
//	shextNoConfirmDel = ini.GetInt(SHEXTNOCONFIRMDEL_KEY, FALSE);
	memset(&shAdmin, 0, sizeof(shAdmin));
	memset(&shUser,  0, sizeof(shUser));

	execConfirm		= ini.GetInt(EXECCONRIM_KEY, FALSE);
	lcid			= ini.GetInt(LCID_KEY, -1);
	waitTick		= ini.GetInt(WAITTICK_KEY, DEFAULT_WAITTICK);
	isAutoSlowIo	= ini.GetInt(ISAUTOSLOWIO_KEY, TRUE);
	speedLevel		= ini.GetInt(SPEEDLEVEL_KEY, SPEED_FULL);
	alwaysLowIo		= ini.GetInt(ALWAYSLOWIO_KEY, FALSE);
	enableOwdel		= ini.GetInt(OWDEL_KEY, FALSE);
	enableAcl		= ini.GetInt(ACL_KEY, FALSE);
	enableStream	= ini.GetInt(STREAM_KEY, FALSE);
	enableVerify	= ini.GetInt(VERIFY_KEY, FALSE);
	useOverlapIo	= ini.GetInt(USEOVERLAPIO_KEY, TRUE);
	usingMD5		= ini.GetInt(USEMD5_KEY, TRUE);
	hashMode		= ini.GetInt(HASHMODE_KEY, usingMD5 ? 0 : 1);
	enableNSA		= ini.GetInt(NSA_KEY, FALSE);
	delDirWithFilter= ini.GetInt(DELDIR_KEY, FALSE);
	enableMoveAttr	= ini.GetInt(MOVEATTR_KEY, FALSE);
	serialMove		= ini.GetInt(SERIALMOVE_KEY, TRUE);
	serialVerifyMove = ini.GetInt(SERIALVERIFYMOVE_KEY, TRUE);
	isReparse		= ini.GetInt(REPARSE_KEY, TRUE);
	isLinkDest		= ini.GetInt(LINKDEST_KEY, FALSE);
	maxLinkHash		= ini.GetInt(MAXLINKHASH_KEY, DEFAULT_LINKHASH);
	allowContFsize	= ini.GetInt(ALLOWCONTFSIZE_KEY, DEFAULT_ALLOWCONTFSIZE);
	isReCreate		= ini.GetInt(RECREATE_KEY, FALSE);
	isExtendFilter	= ini.GetInt(EXTENDFILTER_KEY, FALSE);
	taskbarMode		= ini.GetInt(TASKBARMODE_KEY, 0);
	finishNotify	= ini.GetInt(FINISHNOTIFY_KEY, 1);
	finishNotifyTout = ini.GetInt(FINISHNOTIFYTOUT_KEY, FINISH_NOTIFY_DEFAULT);
	preventSleep	= ini.GetInt(PREVENTSLEEP_KEY, 1);

	infoSpan		= ini.GetInt(INFOSPAN_KEY, DEFAULT_INFOSPAN);
	if (infoSpan < 0 || infoSpan > 2) infoSpan = DEFAULT_INFOSPAN;

	ini.GetStr(WINPOS_KEY, buf, MAX_PATH, "");
	winpos.x   =      (p = strtok(buf,  ", \t")) ? atoi(p) : INVALID_POINTVAL;
	winpos.y   = p && (p = strtok(NULL, ", \t")) ? atoi(p) : INVALID_POINTVAL;
	winsize.cx = p && (p = strtok(NULL, ", \t")) ? atoi(p) : INVALID_SIZEVAL;
	winsize.cy = p && (p = strtok(NULL, ", \t")) ? atoi(p) : INVALID_SIZEVAL;

	ini.GetStr(DRIVEMAP_KEY, driveMap, sizeof(driveMap), "");

	ini.GetStr(STATUSFONT_KEY, buf, MAX_HISTORY_CHAR_BUF, "");
	IniStrToW(buf, statusFont);
	statusFontSize = ini.GetInt(STATUSFONTSIZE_KEY, 0);

/* logfile */
	ini.GetStr(LOGFILE_KEY, buf, MAX_PATH, DEFAULT_FASTCOPYLOG);
	IniStrToW(buf, wbuf.Buf());
	if (wcschr(wbuf.s(), '\\') == NULL) {
		Wstr	wname(wbuf);
		MakePathW(wbuf.Buf(), userDir, wname.s());
	}
	errLogPath = wcsdup(wbuf.s());

/* History */
	for (i=0; i < sizeof(section_array) / sizeof(char *); i++) {
		char	*section_p = section_array[i];
		bool	&is_filter = is_filter_array[i];
		WCHAR	**&history = *history_array[i];

		ini.SetSection(section_p);
		history = (WCHAR **)calloc(maxHistory, sizeof(WCHAR *));
		for (j=0; j < maxHistory + 30; j++) {
			sprintf(key, "%d", j);
			if (j < maxHistory) {
				if (is_filter) {
					GetFilterStr(key, buf, wbuf.Buf());
				} else {
					ini.GetStr(key, buf, MAX_HISTORY_CHAR_BUF);
					IniStrToW(buf, wbuf.Buf());
				}
				history[j] = wcsdup(wbuf.s());
			}
			else if (!ini.DelKey(key))
				break;
		}
	}

/* Job */
	for (i=0; i < JOB_MAX; i++) {
		Job		job;

		sprintf(section, FMT_JOB_KEY, i);
		ini.SetSection(section);

		if (ini.GetStr(TITLE_KEY, buf, MAX_HISTORY_CHAR_BUF) <= 0)
			break;
		IniStrToW(buf, wbuf.Buf());
		job.title = wcsdup(wbuf.s());

		ini.GetStr(SRC_KEY, buf, MAX_HISTORY_CHAR_BUF);
		IniStrToW(buf, wbuf.Buf());
		job.src = wcsdup(wbuf.s());

		ini.GetStr(DST_KEY, buf, MAX_HISTORY_CHAR_BUF);
		IniStrToW(buf, wbuf.Buf());
		job.dst = wcsdup(wbuf.s());

		ini.GetStr(CMD_KEY, buf, MAX_HISTORY_CHAR_BUF);
		IniStrToW(buf, wbuf.Buf());
		job.cmd = wcsdup(wbuf.s());

		GetFilterStr(INCLUDE_KEY, buf, wbuf.Buf());
		job.includeFilter = wcsdup(wbuf.s());
		GetFilterStr(EXCLUDE_KEY, buf, wbuf.Buf());
		job.excludeFilter = wcsdup(wbuf.s());

		ini.GetStr(FROMDATE_KEY, buf, MAX_HISTORY_CHAR_BUF);
		IniStrToW(buf, wbuf.Buf());
		job.fromDateFilter = wcsdup(wbuf.s());
		ini.GetStr(TODATE_KEY, buf, MAX_HISTORY_CHAR_BUF);
		IniStrToW(buf, wbuf.Buf());
		job.toDateFilter = wcsdup(wbuf.s());

		ini.GetStr(MINSIZE_KEY, buf, MAX_HISTORY_CHAR_BUF);
		IniStrToW(buf, wbuf.Buf());
		job.minSizeFilter = wcsdup(wbuf.s());
		ini.GetStr(MAXSIZE_KEY, buf, MAX_HISTORY_CHAR_BUF);
		IniStrToW(buf, wbuf.Buf());
		job.maxSizeFilter = wcsdup(wbuf.s());

		job.estimateMode = ini.GetInt(ESTIMATE_KEY, 0);
		job.diskMode = ini.GetInt(DISKMODE_KEY, 0);
		job.ignoreErr = ini.GetInt(IGNORE_ERR_KEY, TRUE);
		job.enableOwdel = ini.GetInt(OWDEL_KEY, FALSE);
		job.enableAcl = ini.GetInt(ACL_KEY, FALSE);
		job.enableStream = ini.GetInt(STREAM_KEY, FALSE);
		job.enableVerify = ini.GetInt(VERIFY_KEY, FALSE);
		job.isFilter = ini.GetInt(FILTER_KEY, FALSE);
		job.bufSize = ini.GetInt(BUFSIZE_KEY, DEFAULT_BUFSIZE);
		if (job.bufSize < maxTransSize * BUFIO_SIZERATIO) {
			job.bufSize = maxTransSize * BUFIO_SIZERATIO;
		}

		AddJobW(&job);
	}

/* FinAct */
	for (i=0; i < FINACT_MAX; i++) {
		FinAct	act;

		sprintf(buf, FMT_FINACT_KEY, i);
		ini.SetSection(buf);

		if (ini.GetStr(TITLE_KEY, buf, MAX_HISTORY_CHAR_BUF) <= 0)
			break;
		IniStrToW(buf, wbuf.Buf());
		act.title = wcsdup(wbuf.Buf());

		ini.GetStr(SOUND_KEY, buf, MAX_HISTORY_CHAR_BUF);
		IniStrToW(buf, wbuf.Buf());
		act.sound = wcsdup(wbuf.s());

		ini.GetStr(CMD_KEY, buf, MAX_HISTORY_CHAR_BUF);
		IniStrToW(buf, wbuf.Buf());
		act.command = wcsdup(wbuf.s());

		act.flags = ini.GetInt(FLAGS_KEY, 0);

		if (ini.GetStr(SHUTDOWNTIME_KEY, buf, MAX_HISTORY_CHAR_BUF) > 0) {
			act.shutdownTime = strtol(buf, 0, 10);
		}
		AddFinActW(&act);
	}

	if (::GetFileAttributesW(ini.GetIniFileNameW()) == 0xffffffff) {
		WriteIni();
	}
	needIniConvert = FALSE;

	return	TRUE;
}

BOOL Cfg::PostReadIni(void)
{
	DWORD id_list[] = {
		IDS_FINACT_NORMAL, IDS_FINACT_SUSPEND, IDS_FINACT_HIBERNATE, IDS_FINACT_SHUTDOWN, 0
	};
	DWORD flags_list[] = {
		0,				   FinAct::SUSPEND,	   FinAct::HIBERNATE,	 FinAct::SHUTDOWN,    0
	};

	for (int i=0; id_list[i]; i++) {
		if (i >= finActMax) {
			FinAct	act;
			act.flags = flags_list[i] | FinAct::BUILTIN;
			if (i >= 1) act.shutdownTime = 60;
			act.SetString(LoadStrW(id_list[i]), L"", L"");
			AddFinActW(&act);
		}
		if (finActArray[i]->flags & FinAct::BUILTIN) {
			free(finActArray[i]->title);
			finActArray[i]->title = wcsdup(LoadStrW(id_list[i]));
		}
	}

	return	TRUE;
}

BOOL Cfg::WriteIni(void)
{
	ini.StartUpdate();

	ini.SetSection(MAIN_SECTION);

	ini.SetInt(INI_VERSION_KEY, iniVersion);
	ini.KeyToTop(INI_VERSION_KEY);

	ini.SetInt(BUFSIZE_KEY, bufSize);
	ini.SetInt(MAXRUNNUM_KEY, maxRunNum);
	ini.SetInt(MAXTRANSSIZE_KEY, maxTransSize);
	ini.SetInt(MAXOVLNUM_KEY, maxOvlNum);
//	ini.SetInt(MAXOVLSIZE_KEY, maxOvlSize);

	ini.SetInt(MINSECTOR_KEY, minSectorSize);

//	ini.SetInt(MAXOPENFILES_KEY, maxOpenFiles);
	ini.SetInt(NONBUFMINSIZENTFS_KEY, nbMinSizeNtfs);
	ini.SetInt(NONBUFMINSIZEFAT_KEY, nbMinSizeFat);
	ini.SetInt(TIMEDIFFGRACE_KEY, timeDiffGrace);

	ini.SetInt(ISREADOSBUF_KEY, isReadOsBuf);
	//ini.SetInt(WRITESHAREOPEN_KEY, isWShareOpen);

	ini.SetInt(MAX_HISTORY_KEY, maxHistoryNext);
	ini.SetInt(COPYMODE_KEY, copyMode);
//	ini.SetInt(COPYFLAGS_KEY, copyFlags);
//	ini.SetInt(COPYUNFLAGS_KEY, copyUnFlags);

	ini.SetInt(SKIPEMPTYDIR_KEY, skipEmptyDir);
	ini.SetInt(IGNORE_ERR_KEY, ignoreErr);
	ini.SetInt(ESTIMATE_KEY, estimateMode);
	ini.SetInt(DISKMODE_KEY, diskMode);
	ini.SetInt(NETDRVMODE_KEY, netDrvMode);
//	ini.SetInt(ACLRESET_KEY, aclReset);

	ini.SetInt(ISTOPLEVEL_KEY, isTopLevel);
	ini.SetInt(ISERRLOG_KEY, isErrLog);
	ini.SetInt(ISUTF8LOG_KEY, isUtf8Log);
	ini.SetInt(FILELOGMODE_KEY, fileLogMode);
	ini.SetInt(FILELOGFLAGS_KEY, fileLogFlags);
	ini.SetInt(ACLERRLOG_KEY, aclErrLog);
	ini.SetInt(STREAMERRLOG_KEY, streamErrLog);
//	ini.SetInt(DEBUGFLAGS_KEY, debugFlags);
//	ini.SetInt(ISRUNASBUTTON_KEY, isRunasButton);
	ini.SetInt(ISSAMEDIRRENAME_KEY, isSameDirRename);
//	ini.SetInt(SHEXTAUTOCLOSE_KEY, shextAutoClose);
//	ini.SetInt(SHEXTTASKTRAY_KEY, shextTaskTray);
//	ini.SetInt(SHEXTNOCONFIRM_KEY, shextNoConfirm);
//	ini.SetInt(SHEXTNOCONFIRMDEL_KEY, shextNoConfirmDel);
	ini.SetInt(EXECCONRIM_KEY, execConfirm);
	ini.SetInt(FORCESTART_KEY, forceStart);
	ini.SetInt(LCID_KEY, lcid);
//	ini.SetInt(WAITTICK_KEY, waitTick);
//	ini.SetInt(ISAUTOSLOWIO_KEY, isAutoSlowIo);
	ini.SetInt(SPEEDLEVEL_KEY, speedLevel);
//	ini.SetInt(ALWAYSLOWIO_KEY, alwaysLowIo);
	ini.SetInt(OWDEL_KEY, enableOwdel);
	ini.SetInt(ACL_KEY, enableAcl);
	ini.SetInt(STREAM_KEY, enableStream);
	ini.SetInt(VERIFY_KEY, enableVerify);
	ini.SetInt(USEOVERLAPIO_KEY, useOverlapIo);
//	ini.SetInt(USEMD5_KEY, usingMD5);
	ini.SetInt(HASHMODE_KEY, hashMode);
	ini.SetInt(NSA_KEY, enableNSA);
	ini.SetInt(DELDIR_KEY, delDirWithFilter);
	ini.SetInt(MOVEATTR_KEY, enableMoveAttr);
	ini.SetInt(SERIALMOVE_KEY, serialMove);
	ini.SetInt(SERIALVERIFYMOVE_KEY, serialVerifyMove);
	ini.SetInt(REPARSE_KEY, isReparse);
//	ini.SetInt(LINKDEST_KEY, isLinkDest);
//	int.SetInt(MAXLINKHASH_KEY, maxLinkHash);
//	int.SetInt(ALLOWCONTFSIZE_KEY, allowContFsize);
//	ini.SetInt(RECREATE_KEY, isReCreate);
	ini.SetInt(EXTENDFILTER_KEY, isExtendFilter);
	ini.SetInt(TASKBARMODE_KEY, taskbarMode);
	ini.SetInt(FINISHNOTIFY_KEY, finishNotify);
//	ini.SetInt(FINISHNOTIFYTOUT_KEY, finishNotifyTout);
	ini.SetInt(PREVENTSLEEP_KEY, preventSleep);
	ini.SetInt(INFOSPAN_KEY, infoSpan);

	char	val[256];
	sprintf(val, "%d,%d,%d,%d", winpos.x, winpos.y, winsize.cx, winsize.cy);
	ini.SetStr(WINPOS_KEY, val);
	ini.SetStr(DRIVEMAP_KEY, driveMap);

	WtoU8(statusFont, val, sizeof(val));
	ini.SetStr(STATUSFONT_KEY, val);
	ini.SetInt(STATUSFONTSIZE_KEY, statusFontSize);

	char	*section_array[] = {	SRC_HISTORY, DST_HISTORY, DEL_HISTORY,
									INC_HISTORY, EXC_HISTORY,
									FROMDATE_HISTORY, TODATE_HISTORY,
									MINSIZE_HISTORY, MAXSIZE_HISTORY };
	WCHAR	***history_array[] = {	&srcPathHistory, &dstPathHistory, &delPathHistory,
									&includeHistory, &excludeHistory,
									&fromDateHistory, &toDateHistory,
									&minSizeHistory, &maxSizeHistory };
	int		i, j;
	char	key[100];
	DynBuf	buf(MAX_HISTORY_CHAR_BUF);

	for (i=0; i < sizeof(section_array) / sizeof(char *); i++) {
		char	*&section = section_array[i];
		WCHAR	**&history = *history_array[i];

		ini.SetSection(section);
		for (j=0; j < maxHistory; j++) {
			sprintf(key, "%d", j);
			WtoU8(history[j], buf, MAX_HISTORY_CHAR_BUF);
			if (j < maxHistoryNext)
				ini.SetStr(key, buf);
			else
				ini.DelKey(key);
		}
	}

	for (i=0; i < jobMax; i++) {
		sprintf(buf, FMT_JOB_KEY, i);
		Job *job = jobArray[i];

		ini.SetSection(buf);

		WtoU8(job->title, buf, MAX_HISTORY_CHAR_BUF);
		ini.SetStr(TITLE_KEY,		buf);

		WtoU8(job->src, buf, MAX_HISTORY_CHAR_BUF);
		ini.SetStr(SRC_KEY,			buf);

		WtoU8(job->dst, buf, MAX_HISTORY_CHAR_BUF);
		ini.SetStr(DST_KEY,			buf);

		WtoU8(job->cmd, buf, MAX_HISTORY_CHAR_BUF);
		ini.SetStr(CMD_KEY,			buf);

		WtoU8(job->includeFilter, buf, MAX_HISTORY_CHAR_BUF);
		ini.SetStr(INCLUDE_KEY,		buf);
		WtoU8(job->excludeFilter, buf, MAX_HISTORY_CHAR_BUF);
		ini.SetStr(EXCLUDE_KEY,		buf);

		WtoU8(job->fromDateFilter, buf, MAX_HISTORY_CHAR_BUF);
		ini.SetStr(FROMDATE_KEY,	buf);
		WtoU8(job->toDateFilter, buf, MAX_HISTORY_CHAR_BUF);
		ini.SetStr(TODATE_KEY,	buf);

		WtoU8(job->minSizeFilter, buf, MAX_HISTORY_CHAR_BUF);
		ini.SetStr(MINSIZE_KEY,	buf);
		WtoU8(job->maxSizeFilter, buf, MAX_HISTORY_CHAR_BUF);
		ini.SetStr(MAXSIZE_KEY,	buf);

		ini.SetInt(ESTIMATE_KEY,	job->estimateMode);
		ini.SetInt(DISKMODE_KEY,	job->diskMode);
		ini.SetInt(IGNORE_ERR_KEY,	job->ignoreErr);
		ini.SetInt(OWDEL_KEY,		job->enableOwdel);
		ini.SetInt(ACL_KEY,			job->enableAcl);
		ini.SetInt(STREAM_KEY,		job->enableStream);
		ini.SetInt(VERIFY_KEY,		job->enableVerify);
		ini.SetInt(FILTER_KEY,		job->isFilter);
		ini.SetInt(BUFSIZE_KEY,		job->bufSize);
	}
	sprintf(buf, FMT_JOB_KEY, i);
	ini.DelSection(buf);

	for (i=0; i < finActMax; i++) {
		sprintf(buf, FMT_FINACT_KEY, i);
		FinAct *finAct = finActArray[i];

		ini.SetSection(buf);

		WtoU8(finAct->title, buf, MAX_HISTORY_CHAR_BUF);
		ini.SetStr(TITLE_KEY,		buf);

		WtoU8(finAct->sound, buf, MAX_HISTORY_CHAR_BUF);
		ini.SetStr(SOUND_KEY,		buf);

		WtoU8(finAct->command, buf, MAX_HISTORY_CHAR_BUF);
		ini.SetStr(CMD_KEY,			buf);

		ini.SetInt(SHUTDOWNTIME_KEY,	finAct->shutdownTime);
		ini.SetInt(FLAGS_KEY,			finAct->flags);
	}
	sprintf(buf, FMT_FINACT_KEY, i);
	ini.DelSection(buf);

	return	ini.EndUpdate();
}

BOOL Cfg::EntryPathHistory(WCHAR *src, WCHAR *dst)
{
	WCHAR	*path_array[] = { src, dst };
	WCHAR	***history_array[] = { &srcPathHistory, &dstPathHistory };

	return	EntryHistory(path_array, history_array, 2);
}

BOOL Cfg::EntryDelPathHistory(WCHAR *del)
{
	WCHAR	*path_array[] = { del };
	WCHAR	***history_array[] = { &delPathHistory };

	return	EntryHistory(path_array, history_array, 1);
}

BOOL Cfg::EntryFilterHistory(WCHAR *inc, WCHAR *exc, WCHAR *from, WCHAR *to, WCHAR *min, WCHAR *max)
{
	WCHAR	*path_array[] = { inc, exc, from, to, min, max };
	WCHAR	***history_array[] = {
		&includeHistory, &excludeHistory, &fromDateHistory, &toDateHistory,
		&minSizeHistory, &maxSizeHistory
	};

	return	EntryHistory(path_array, history_array, 6);
}

BOOL Cfg::EntryHistory(WCHAR **path_array, WCHAR ****history_array, int max)
{
	BOOL	ret = TRUE;
	WCHAR	*target_path;

	for (int i=0; i < max; i++) {
		int		idx;
		WCHAR	*&path = path_array[i];
		WCHAR	**&history = *history_array[i];

		if (!path || wcslen(path) >= MAX_HISTORY_BUF || path[0] == 0) {
			ret = FALSE;
			continue;
		}
		for (idx=0; idx < maxHistory; idx++) {
			if (wcsicmp(path, history[idx]) == 0)
				break;
		}
		if (idx) {
			if (idx == maxHistory) {
				target_path = wcsdup(path);
				free(history[--idx]);
			}
			else {
				target_path = history[idx];
			}
			memmove(history + 1, history, idx * sizeof(WCHAR *));
			history[0] = target_path;
		}
	}
	return	ret;
}

BOOL Cfg::IniStrToW(char *str, WCHAR *wstr)
{
	int		len = (int)strlen(str) + 1;

	if (needIniConvert && *str == '|') { // old style
		hexstr2bin(str + 1, (BYTE *)wstr, len, &len);
	}
	else {
		if (needIniConvert && !IsUTF8(str)) {
			AtoW(str, wstr, len);
		} else {
			U8toW(str, wstr, len);
		}
	}

	return	TRUE;
}

int Cfg::SearchJobW(const WCHAR *title)
{
	for (int i=0; i < jobMax; i++) {
		if (wcsicmp(jobArray[i]->title, title) == 0)
			return	i;
	}
	return	-1;
}

BOOL Cfg::AddJobW(const Job *job)
{
	if (job->title[0] == 0 || wcslen(job->src) >= MAX_HISTORY_BUF || job->src[0] == 0)
		return	FALSE;

	int idx = SearchJobW(job->title);

	if (idx >= 0) {
		delete jobArray[idx];
		jobArray[idx] = new Job(*job);
		return TRUE;
	}

#define ALLOC_JOB	100
	if ((jobMax % ALLOC_JOB) == 0)
		jobArray = (Job **)realloc(jobArray, sizeof(Job *) * (jobMax + ALLOC_JOB));

	for (idx=0; idx < jobMax; idx++) {
		if (wcsicmp(jobArray[idx]->title, job->title) > 0)
			break;
	}
	memmove(jobArray + idx + 1, jobArray + idx, sizeof(Job *) * (jobMax++ - idx));
	jobArray[idx] = new Job(*job);
	return	TRUE;
}

BOOL Cfg::DelJobW(const WCHAR *title)
{
	int idx = SearchJobW(title);
	if (idx == -1)
		return	FALSE;

	delete jobArray[idx];
	memmove(jobArray + idx, jobArray + idx + 1, sizeof(Job *) * (--jobMax - idx));
	return	TRUE;
}

int Cfg::SearchFinActW(const WCHAR *title, BOOL is_cmdline)
{
	for (int i=0; i < finActMax; i++) {
		if (wcsicmp(finActArray[i]->title, title) == 0)
			return	i;
	}

	if (is_cmdline) {
		WCHAR	*std_title[] = { STANDBY_STR, HIBERNATE_STR, SHUTDOWN_STR, 0 };
		for (int i=0; std_title[i]; i++) {
			if (wcsicmp(std_title[i], title) == 0) return i + 1;
		}
	}

	return	-1;
}

BOOL Cfg::AddFinActW(const FinAct *finAct)
{
	if (finAct->title[0] == 0)
		return	FALSE;

	int		idx = SearchFinActW(finAct->title);

	if (idx >= 0) {
		DWORD builtin_flag = finActArray[idx]->flags & FinAct::BUILTIN;
		delete finActArray[idx];
		finActArray[idx] = new FinAct(*finAct);
		finActArray[idx]->flags |= builtin_flag;
		return TRUE;
	}

#define ALLOC_FINACT	100
	if ((finActMax % ALLOC_FINACT) == 0)
		finActArray =
			(FinAct **)realloc(finActArray, sizeof(FinAct *) * (finActMax + ALLOC_FINACT));

	finActArray[finActMax++] = new FinAct(*finAct);
	return	TRUE;
}

BOOL Cfg::DelFinActW(const WCHAR *title)
{
	int idx = SearchFinActW(title);
	if (idx == -1)
		return	FALSE;

	delete finActArray[idx];
	memmove(finActArray + idx, finActArray + idx + 1, sizeof(FinAct *) * (--finActMax - idx));
	return	TRUE;
}

