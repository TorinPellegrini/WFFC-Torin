#pragma once
#include <afxdialogex.h>
#include "afxwin.h"
#include "pch.h"
#include "resource.h"


class ToolMain;

class PositionDialogue :
    public CDialogEx
{
    DECLARE_DYNAMIC(PositionDialogue)
public:
    PositionDialogue(CWnd* pParent = NULL);
    virtual ~PositionDialogue();

    afx_msg void End();

    void SetObjectData(ToolMain* toolmain, float PosX, float PosY, float PosZ);

#ifdef AFX_DESIGN_TIME
    enum{ IDD = IDD_DIALOG2 };
#endif

    afx_msg void TranslateNegativeX();
    afx_msg void TranslateNegativeY();
    afx_msg void TranslateNegativeZ();
   
    afx_msg void TranslatePositiveX();
    afx_msg void TranslatePositiveY();
    afx_msg void TranslatePositiveZ();

    DECLARE_MESSAGE_MAP()

protected:

    virtual void DoDataExchange(CDataExchange* pDX) override;
    CStatic transX, transY, transZ;

private:
    float posX, posY, posZ;

    ToolMain* m_toolMain;
};

