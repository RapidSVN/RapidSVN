#ifndef _EXCEPTIONS_H_INCLUDED_
#define _EXCEPTIONS_H_INCLUDED_

class VSvnEx
{
public:
	wxString	estr;
	long		code;

public:
					VSvnEx( const wxString&__estr = "",
							const long __code = -1 );

					VSvnEx( const VSvnEx& rhs )
					{
						*this = rhs;
					}

	VSvnEx&			operator=( const VSvnEx& rhs );
	
	void			what( const wxString& __estr );

	virtual 
	const wxChar*	what() const
					{
						return estr.c_str();
					}
						
	const wxString&	str( void ) const
					{
						return estr;
					}
};
#endif