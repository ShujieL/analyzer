#ifndef ROOT_THaAnalysisObject
#define ROOT_THaAnalysisObject

//////////////////////////////////////////////////////////////////////////
//
// THaAnalysisObject
//
//////////////////////////////////////////////////////////////////////////

#include "TNamed.h"
#include <vector>
#include <string>
#include <cstdio>

class TDatime;
struct VarDef;
struct RVarDef;
struct TagDef;

class THaAnalysisObject : public TNamed {
  
public:
  enum EStatus { kOK, kNotinit, kInitError };
  enum EType   { kVarDef, kRVarDef };
  enum EMode   { kDefine, kDelete };

  virtual ~THaAnalysisObject();
  
  virtual const char*  GetDBFileName() const     { return GetPrefix(); }
          Int_t        GetDebug() const          { return fDebug; }
          const char*  GetPrefix() const         { return fPrefix; }
          EStatus      Init();
  virtual EStatus      Init( const TDatime& run_time );
          Bool_t       IsInit() const            { return IsOK(); }
          Bool_t       IsOK() const              { return (fStatus == kOK); }
  virtual void         SetDebug( Int_t level )   { fDebug = level; }
  virtual void         SetName( const char* name );
  virtual void         SetNameTitle( const char* name, const char* title );
          EStatus      Status() const            { return fStatus; }

  // Static function to provide easy access to database files
  // from CINT scripts etc.
  static  FILE*   OpenFile( const char* name, const TDatime& date,
			    const char* here = "OpenFile()",
			    const char* filemode = "r", 
			    const int debug_flag = 1);

  // Access functions for reading tag/value pairs from database files
  static  Int_t   LoadDBvalue( FILE* file, const TDatime& date, 
			       const char* tag, Double_t& value );
  static   Int_t  LoadDB( FILE* file, const TDatime& date, 
			  const TagDef* tags, const char* prefix="" );

  // Angle transformation utility functions
  static  void    GeoToSph( Double_t  th_geo, Double_t  ph_geo,
			    Double_t& th_sph, Double_t& ph_sph );
  static  void    SphToGeo( Double_t  th_sph, Double_t  ph_sph,
			    Double_t& th_geo, Double_t& ph_geo );

protected:

  // General status variables
  char*           fPrefix;    // Name prefix for global variables
  EStatus         fStatus;    // Initialization status flag
  Int_t           fDebug;     // Debug level
  bool            fIsInit;    // Flag indicating that ReadDatabase called.
  bool            fIsSetup;   // Flag indicating that Setup called.

  virtual Int_t        DefineVariables( EMode mode = kDefine )
     { return kOK; }

          Int_t        DefineVarsFromList( const VarDef* list, 
					   EMode mode = kDefine ) const
     { return DefineVarsFromList( list, kVarDef, mode ); }

          Int_t        DefineVarsFromList( const RVarDef* list, 
					   EMode mode = kDefine ) const
     { return DefineVarsFromList( list, kRVarDef, mode ); }
 
          Int_t        DefineVarsFromList( const void* list, 
					   EType type, EMode mode ) const;

  virtual const char*  Here( const char* ) const;
          void         MakePrefix( const char* basename );
  virtual void         MakePrefix() = 0;
  virtual FILE*        OpenFile( const TDatime& date )
     { return OpenFile(GetDBFileName(), date, Here("OpenFile()")); }
  virtual Int_t        ReadDatabase( FILE* file, const TDatime& date )
     { return kOK; }
  virtual Int_t        ReadRunDatabase( FILE* file, const TDatime& date )
     { return kOK; }
  virtual Int_t        RemoveVariables()
     { return DefineVariables( kDelete ); }

  // Support function for reading database files
  static vector<string> GetDBFileList( const char* name, const TDatime& date,
				       const char* here = "GetDBFileList()" );

  //Only derived classes may construct me
  THaAnalysisObject() : fPrefix(NULL), fStatus(kNotinit), 
    fDebug(0), fIsInit(false), fIsSetup(false) {}
  THaAnalysisObject( const char* name, const char* description );

private:
  // Support fucntions for reading database files
  static Int_t IsDBdate( const string& line, TDatime& date );
  static Int_t IsDBtag ( const string& line, const char* tag, Double_t& value );

  ClassDef(THaAnalysisObject,0)   //ABC for a data analysis object
};

//____________ inline functions _______________________________________________

#endif