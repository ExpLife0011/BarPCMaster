#pragma once

enum EXAMINATION_TYPE
{
	EXT_TYPE_CLEAN,					// ����
	EXT_TYPE_SPEEDUP				// ����
};

enum EXAMINATION_KEYTYPE
{
	EXT_KEYTYPE_DWORD,				// 32λ DWORD ����
	EXT_KEYTYPE_STRING,				// �ַ�������
};

enum EXAMINATION_OPERATION
{
	EXT_OPT_FILES,					// ָ���ļ�
	EXT_OPT_REG32,					// 32λע���
	EXT_OPT_REG64,					// 64λע���
};

enum EXAMINATION_COMPATISON
{
	EXT_COMPT_LT = -2,				// С��
	EXT_COMPT_LTE,					// С�ڵ���
	EXT_COMPT_EQUAL,				// ����
	EXT_COMPT_GTE,					// ���ڵ���
	EXT_COMPT_GT,					// ����
};
