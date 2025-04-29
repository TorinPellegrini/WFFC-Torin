#include "PositionDialogue.h"
#include <string>
#include "ToolMain.h"
#include "stdafx.h"
#include "resource.h"


IMPLEMENT_DYNAMIC(PositionDialogue, CDialogEx)

BEGIN_MESSAGE_MAP(PositionDialogue, CDialogEx)
	ON_COMMAND(IDOK, &PositionDialogue::End)
	ON_COMMAND(IDCANCEL, &PositionDialogue::End)
	ON_COMMAND(IDC_NEGATIVEX, &PositionDialogue::TranslateNegativeX)
	ON_COMMAND(IDC_NEGATIVEY, &PositionDialogue::TranslateNegativeY)
	ON_COMMAND(IDC_NEGATIVEZ, &PositionDialogue::TranslateNegativeZ)
	ON_COMMAND(IDC_POSITIVEZ, &PositionDialogue::TranslatePositiveZ)
	ON_COMMAND(IDC_POSITIVEY, &PositionDialogue::TranslatePositiveY)
	ON_COMMAND(IDC_POSITIVEX, &PositionDialogue::TranslatePositiveX)
END_MESSAGE_MAP()

void PositionDialogue::TranslateNegativeX()
{
	m_toolMain->translateX(false);
}

void PositionDialogue::TranslateNegativeY()
{
	m_toolMain->translateY(false);

}

void PositionDialogue::TranslateNegativeZ()
{
	m_toolMain->translateZ(false);

}

void PositionDialogue::TranslatePositiveX()
{
	m_toolMain->translateX(true);

}

void PositionDialogue::TranslatePositiveY()
{
	m_toolMain->translateY(true);

}

void PositionDialogue::TranslatePositiveZ()
{
	m_toolMain->translateZ(true);

}

void PositionDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_XPOSITION, transX);
	DDX_Control(pDX, IDC_YPOSITION, transY);
	DDX_Control(pDX, IDC_ZPOSITION, transZ);
}

PositionDialogue::PositionDialogue(CWnd* pParent) : CDialogEx(ID_EDIT_POSITIONTOOL, pParent)
{
}

PositionDialogue::~PositionDialogue()
{
}

void PositionDialogue::End()
{
	DestroyWindow();
}

void PositionDialogue::SetObjectData(ToolMain* toolmain, float PosX, float PosY, float PosZ)
{
	m_toolMain = toolmain;

	transX.SetWindowTextW(std::to_wstring((int)PosX).c_str());
	transY.SetWindowTextW(std::to_wstring((int)PosY).c_str());
	transZ.SetWindowTextW(std::to_wstring((int)PosZ).c_str());
}
