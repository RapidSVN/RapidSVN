#ifndef _VSVN_APP_H_INCLUDED_
#define _VSVN_APP_H_INCLUDED_

#include "filelist_ctrl.h"
#include "folder_browser.h"
#include "utils.h"
#include "rapidsvn.h"

class VSvnFrame;

class VsvnApp: public wxApp
{
	virtual bool OnInit();
	virtual int OnExit();

public:
	VSvnFrame		*appFrame;

	VSvnFrame*		GetAppFrame()
					{
						return appFrame;
					}
};

DECLARE_APP(VsvnApp)

#endif