/* 
/ freexl_internals.h
/
/ internal declarations
/
/ version  2.1, 2021 June 10
/
/ Author: Sandro Furieri a.furieri@lqt.it
/
/ ------------------------------------------------------------------------------
/ 
/ Version: MPL 1.1/GPL 2.0/LGPL 2.1
/ 
/ The contents of this file are subject to the Mozilla Public License Version
/ 1.1 (the "License"); you may not use this file except in compliance with
/ the License. You may obtain a copy of the License at
/ http://www.mozilla.org/MPL/
/ 
/ Software distributed under the License is distributed on an "AS IS" basis,
/ WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
/ for the specific language governing rights and limitations under the
/ License.
/
/ The Original Code is the FreeXL library
/
/ The Initial Developer of the Original Code is Alessandro Furieri
/ 
/ Portions created by the Initial Developer are Copyright (C) 2011-2023
/ the Initial Developer. All Rights Reserved.
/ 
/ Contributor(s):
/ Brad Hards
/ 
/ Alternatively, the contents of this file may be used under the terms of
/ either the GNU General Public License Version 2 or later (the "GPL"), or
/ the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
/ in which case the provisions of the GPL or the LGPL are applicable instead
/ of those above. If you wish to allow use of your version of this file only
/ under the terms of either the GPL or the LGPL, and not to allow others to
/ use your version of this file under the terms of the MPL, indicate your
/ decision by deleting the provisions above and replace them with the notice
/ and other provisions required by the GPL or the LGPL. If you do not delete
/ the provisions above, a recipient may use your version of this file under
/ the terms of any one of the MPL, the GPL or the LGPL.
/ 
*/

#define BIFF_MAX_FORMAT	2048
#define BIFF_MAX_XF	8192

#define FREEXL_MAGIC_START	1675431287
#define FREEXL_MAGIC_END	178213456

/* BIFF record types */
#define BIFF_EOF		0x000A
#define BIFF_DATEMODE		0x0022
#define BIFF_FILEPASS		0x002F
#define BIFF_CODEPAGE		0x0042
#define BIFF_SHEET		0x0085
#define BIFF_SST		0x00FC
#define BIFF_DIMENSION		0x0200
#define BIFF_SHEETSOFFSET	0x008E
#define BIFF_BOF_2		0x0009
#define BIFF_BOF_3		0x0209
#define BIFF_BOF_4		0x0409
#define BIFF_BOF		0x0809
#define BIFF_CONTINUE		0x003C
#define BIFF_FORMAT_2		0x001E
#define BIFF_FORMAT		0x041E
#define BIFF_XF_2		0x0043
#define BIFF_XF_3		0x0243
#define BIFF_XF_4		0x0443
#define BIFF_XF			0x00E0
#define BIFF_INTEGER_2		0x0002
#define BIFF_NUMBER_2		0x0003
#define BIFF_NUMBER		0x0203
#define BIFF_LABEL_2		0x0004
#define BIFF_LABEL		0x0204
#define BIFF_LABEL_SST		0x00FD
#define BIFF_RK			0x027E
#define BIFF_MULRK		0x00BD
#define BIFF_BOOLERR_2	0x0005
#define BIFF_BOOLERR	0x0205

/* XLSX data types */
#define XLSX_NULL		1
#define XLSX_INTEGER	2
#define XLSX_DOUBLE		3
#define XLSX_STR_INDEX	4
#define XLSX_STRING		5
#define XLSX_DATETIME	6

#define XLSX_DATE_NONE		0
#define XLSX_DATE_SIMPLE	1
#define XLSX_TIME_SIMPLE	2
#define XLSX_DATE_AND_TIME	3

/* ODS data types */
#define ODS_VOID		1
#define ODS_FLOAT		2
#define ODS_CURRENCY	3
#define ODS_PERCENTAGE	4
#define ODS_BOOLEAN		5
#define ODS_DATE		6
#define ODS_TIME		7
#define ODS_STRING		8
#define ODS_INTEGER		9

typedef union biff_word
{
    unsigned char bytes[2];
    unsigned short value;
} biff_word16;

typedef union biff_double_word
{
    unsigned char bytes[4];
    unsigned int value;
    int signed_value;
} biff_word32;

typedef union biff_float_word
{
    unsigned char bytes[8];
    double value;
} biff_float;


typedef struct cfbf_header_struct
{
/*
 * a struct wrapping the CFBF
 * Compound File Binary Format
 *
 * this one is a Microsoft format used e.g. by Office,
 * and more or less is intended to implement a multi-stream
 * arch within a single file
 * the whole struct is a file-system like FAT (File Allocation Table)
 */
    unsigned char signature[8];	/* magic signature */
    unsigned char classid[16];	/* Classid [usually zero] */
    biff_word16 minor_version;	/* minor version: unused */
    biff_word16 major_version;	/* 3 [512 bytes/sector] or 4 [4096 bytes/sector] */
    biff_word16 byte_order;	/* endiannes [always little-endian] */
    biff_word16 sector_shift;	/* 9=512 bytes/sector or 12=4096 bytes/sector */
    biff_word16 mini_sector_shift;	/* 6=64 bytes/sector */
    biff_word16 reserved1;	/* unused, ZERO */
    biff_word32 reserved2;	/* unused, ZERO */
    biff_word32 directory_sectors;	/* usually ZERO */
    biff_word32 fat_sectors;	/* #sectors in FAT chain */
    biff_word32 directory_start;	/* sector index for directory */
    biff_word32 transaction_signature;	/* usually ZERO */
    biff_word32 mini_cutoff;	/* tipically 4096 */
    biff_word32 mini_fat_start;	/* sector index for mini-FAT chain */
    biff_word32 mini_fat_sectors;	/* #sectors in mini-FAT chain */
    biff_word32 difat_start;	/* sector index for first DoubleIndirect-FAT */
    biff_word32 difat_sectors;	/* #sectors for DIFAT chain */
    unsigned char fat_sector_map[436];	/* first 109 FAT sectors */
} cfbf_header;

typedef struct cfbf_dir_entry_struct
{
/* a struct representing a CFBF Directory entry */
    char name[64];		/* file name */
    biff_word16 name_size;	/* file name size */
    unsigned char type;		/* 1=directory; 2=file, 5=root */
    unsigned char node_color;	/* 0=red; 1=black */
    biff_word32 previous;	/* previuous item index */
    biff_word32 next;		/* next item index */
    biff_word32 child;		/* first child index */
    unsigned char classid[16];	/* Classid [unused] */
    biff_word32 state;		/* state bits [unused] */
    biff_word32 timestamp_1;	/* timestamp (1) [unused] */
    biff_word32 timestamp_2;	/* timestamp (2) [unused] */
    biff_word32 timestamp_3;	/* timestamp (3) [unused] */
    biff_word32 timestamp_4;	/* timestamp (4) [unused] */
    biff_word32 start_sector;	/* start sector */
    biff_word32 size;		/* actual file-size */
    biff_word32 extra_size;	/* extra size (> 2GB) [ignored] */
} cfbf_dir_entry;

typedef struct biff_string_table_struct
{
/*
 * in BIFF8 a Shared String Table (SST) exists:
 * any Text String referenced by the whole Workbook will 
 * be stored only once on the SST, and then Text Cells will 
 * simply refer the corresponding SST entry
 */
    unsigned int string_count;	/* how many strings are into the SST */
    char **utf8_strings;	/* the String Array [UTF-8] */
    unsigned int current_index;	/* array index for currently parsed string */
    char *current_utf16_buf;	/* current UTF-16 buffer */
    unsigned int current_utf16_len;	/* current UTF-16 length */
    unsigned int current_utf16_off;	/* current UTF-16 offset */
    unsigned int current_utf16_skip;	/* bytes to be skipped after the current string */
    unsigned int next_utf16_skip;	/* remaining bytes to be skipped in the next record */
} biff_string_table;

typedef struct biff_cell_value_struct
{
/* a struct representing a Cell value */
    unsigned char type;
    union multivalue_cell
    {
	int int_value;
	double dbl_value;
	char *text_value;
	const char *sst_value;
    } value;
} biff_cell_value;

typedef struct biff_sheet_struct
{
/* a strunct representing a BIFF Sheet */
    unsigned int start_offset;	/* start offset within the stream */
    unsigned char visible;	/* 0x00=visible; 0x01=hidden; 0x02=very-hidden; */
    unsigned char type;		/* 0x00=work-sheet; 0x01=macro-sheet; 0x02=chart; 0x06=VB-module; */
    char *utf8_name;		/* UTF8 name */
    unsigned int rows;		/* number of rows */
    unsigned short columns;	/* number of columns */
    unsigned short maxcols;	/* effective number of columns allocated in the cell_values array */
    biff_cell_value *cell_values;	/* cell values array */
    int valid_dimension;	/* set to 1=TRUE only when DIMENSION is surely known */
    int already_done;		/* set to 1=TRUE if already loaded in pass #1 */
    struct biff_sheet_struct *next;	/* linked-list pointer */
} biff_sheet;

typedef struct biff_format_struct
{
/* a struct representing DATE/DATETIME/TIME formats */
    unsigned int format_index;
    int is_date;
    int is_datetime;
    int is_time;
} biff_format;

typedef struct fat_entry_struct
{
/* a FAT entry */
    unsigned int current_sector;
    unsigned int next_sector;
    struct fat_entry_struct *next;
} fat_entry;

typedef struct fat_chain_struct
{
/* a struct representing the FAT chain */
    int swap;			/* Endiannes; swap required */
    unsigned short sector_size;	/* sector size */
    unsigned int next_sector;
    unsigned int directory_start;	/* sector index for directory */
    fat_entry *first;
    fat_entry *last;
    fat_entry **fat_array;
    unsigned int fat_array_count;
    unsigned int miniCutOff;
    unsigned int next_sectorMini;
    fat_entry *firstMini;
    fat_entry *lastMini;
    fat_entry **miniFAT_array;
    unsigned int miniFAT_array_count;
    unsigned int miniFAT_start;
    unsigned int miniFAT_len;
    unsigned char *miniStream;	/* the whole mini-stream */
} fat_chain;

typedef struct biff_workbook_struct
{
/* 
 * a struct representing a BIFF Workbook
 * BIFF stands for: Binary Interchange File Format
 * which is a "file format" often used by MS Office
 *
 * an Excel Spreadsheet (.xls) is stored within a CFBF
 * as a Workbook stream [pseudo-file]
 */
    int magic1;			/* magic signature #1 */
    FILE *xls;			/* file handle */
    fat_chain *fat;		/* FAT chain */
    unsigned short cfbf_version;	/* CFBF version */
    unsigned short cfbf_sector_size;	/* CFBF sector size */
    unsigned int start_sector;	/* starting sector for Workbook */
    unsigned int size;		/* total size of the Workbook stream */
    unsigned int current_sector;	/* currently bufferd sector */
    unsigned int bytes_read;	/* total bytes read since start */
    unsigned int current_offset;	/* current stream offset */
    unsigned char sector_buf[8192];	/* currently buffered sector(s) */
    unsigned char *p_in;	/* current buffer pointer */
    unsigned short sector_end;	/* current sector end */
    int sector_ready;		/* 1=yes; 0=no; */
    int ok_bof;			/* valid BOF found (BeginOfFile): -1=expected; 1=yes; 0=no; */
    unsigned short biff_version;	/* BIFF version number */
    unsigned short biff_max_record_size;	/* 2080 or 8224 depending on version */
    unsigned short biff_content_type;	/* the type of the current sub-stream */
    unsigned short biff_code_page;	/* the code-page for the current sub-stream */
    unsigned short biff_book_code_page;	/* the Workbook code-page */
    unsigned short biff_date_mode;	/* the date-mode: 0=1900-Jan-01; 1=1904-Jan-02; */
    int biff_obfuscated;	/* 0=no; 1=yes (encrypted file) */
    iconv_t utf8_converter;	/* ICONV charset converter */
    iconv_t utf16_converter;	/* ICONV charset converter (for UTF-16) */
    unsigned char record[8224];	/* current record */
    unsigned short record_type;	/* current record identifier */
    unsigned short prev_record_type;	/* previous record identifier */
    unsigned int record_size;	/* current record size */
    biff_string_table shared_strings;	/* the SST */
    biff_sheet *first_sheet;	/* SHEET linked list - first item */
    biff_sheet *last_sheet;	/* SHEET linked list - last item */
    biff_sheet *active_sheet;	/* currently active SHEET */
    int second_pass;		/* set to 1=TRUE for pass #2 */
    biff_format format_array[BIFF_MAX_FORMAT];	/* the array for DATE/DATETIME/TIME formats */
    unsigned short max_format_index;	/* max array index [formats] */
    unsigned short biff_xf_array[BIFF_MAX_XF];	/* the array for XF/Format association */
    unsigned short biff_xf_next_index;	/* next XF index */
    int magic2;			/* magic signature #2 */
} biff_workbook;

typedef struct xlsx_cell_struct
{
/* a struct representing a XLSX cell */
    int col_no;
    int type;
    int is_datetime;
    int assigned;
    int int_value;
    double dbl_value;
    int str_index;
    char *text;
    struct xlsx_cell_struct *next;
} xlsx_cell;

typedef struct xlsx_row_struct
{
/* a struct representing a XLSX row of cells */
    int row_no;
    int max_cell;
    xlsx_cell *first;
    xlsx_cell *last;
    struct xlsx_row_struct *next;
} xlsx_row;

typedef struct xlsx_worksheet_struct
{
/* a struct representing a XLSX Worksheet */
    int id;
    char *name;
    char *target;
    xlsx_row *first;
    xlsx_row *last;
    int max_row;
    int max_cell;
    xlsx_row **rows;
    int error;
    char *CharData;
    int CharDataLen;
    int CharDataMax;
    int CharDataStep;
    int RowOk;
    int ColOk;
    int CellValueOk;
    struct xlsx_workbook_struct *wbRef;
    struct xlsx_worksheet_struct *next;
} xlsx_worksheet;

typedef struct xlsx_format_struct
{
/* a struct representing a XLSX format */
    int fmtId;
    int is_datetime;
} xlsx_format;

typedef struct xlsx_style_struct
{
/* a struct representing a XLSX Cell Style */
    int stlId;
    xlsx_format *formatRef;
} xlsx_style;

typedef struct xml_datetime_struct
{
/* a struct intended to store a block of DataTime strings */
#define MAX_DATETIME_STR	128
#define STR_DATETIME_LEN	20
    char datetime[MAX_DATETIME_STR][STR_DATETIME_LEN];
    int next_str;
    struct xml_datetime_struct *next;
} xml_datetime;

typedef struct xlsx_workbook_rel_struct
{
/* a struct for storing WorkbookRels */
    char *id;
    char *target;
    struct xlsx_workbook_rel_struct *next;
} xlsx_workbook_rel;

typedef struct xlsx_workbook_struct
{
/* a struct representing a XLSX Workbook */
    xlsx_workbook_rel *first_rel;
    xlsx_workbook_rel *last_rel;
    xlsx_worksheet *first;
    xlsx_worksheet *last;
    xlsx_worksheet *active_sheet;	/* currently active SHEET */
    int n_strings;
    int xml_strings;
    char **strings;
    int n_formats;
    int next_format;
    xlsx_format *formats;
    int n_styles;
    int next_style;
    xlsx_style *styles;
    xml_datetime *first_date;
    xml_datetime *last_date;
    int error;
    char *WorkbookRelsZipEntry;
    char *SharedStringsZipEntry;
    char *WorkbookZipEntry;
    char *StylesZipEntry;
    char *CharData;
    int CharDataLen;
    int CharDataMax;
    int CharDataStep;
    int RelsOk;
    int SharedStringsOk;
    int WorksheetsOk;
    int StylesOk;
    int FormatsOk;
    int CellStylesOk;
} xlsx_workbook;

typedef struct ods_cell_struct
{
/* a struct representing an ODS cell */
    int col_no;
    int type;
    int assigned;
    int int_value;
    double dbl_value;
    char *txt_value;
    struct ods_cell_struct *next;
} ods_cell;

typedef struct ods_row_struct
{
/* a struct representing an ODS row of cells */
    int row_no;
    int max_cell;
    ods_cell *first;
    ods_cell *last;
    int NextColNo;
    struct ods_row_struct *next;
} ods_row;

typedef struct ods_worksheet_struct
{
/* a struct representing an ODS Worksheet */
    int id;
    char *name;
    int n_columns;
    ods_row *first;
    ods_row *last;
    int max_row;
    int max_cell;
    ods_row **rows;
    int RowOk;
    int ColOk;
    int CellValueOk;
    int NextRowNo;
    struct ods_worksheet_struct *next;
} ods_worksheet;

typedef struct ods_workbook_struct
{
/* a struct representing an ODS Workbook */
    ods_worksheet *first;
    ods_worksheet *last;
    ods_worksheet *active_sheet;	/* currently active SHEET */
    xml_datetime *first_date;
    xml_datetime *last_date;
    int error;
    char *ContentZipEntry;
    char *CharData;
    int CharDataLen;
    int CharDataMax;
    int CharDataStep;
    int ContentOk;
    int NextWorksheetId;
} ods_workbook;

typedef struct freexl_handle_struct
{
/* 
 * a generic handle to an XLS or XLSX or ODS spreadsheet
 */
    biff_workbook *xls_handle;
    xlsx_workbook *xlsx_handle;
    ods_workbook *ods_handle;
} freexl_handle;
