#ifndef ROOT_THaVDCPlane
#define ROOT_THaVDCPlane

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// THaVDCPlane                                                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "THaSubDetector.h"
#include "THaVDCWire.h"
#include "THaVDCCluster.h"
#include "TClonesArray.h"
#include "THaVDCHit.h"
#include <cassert>

namespace VDC {
  class TimeToDistConv;
}

class THaEvData;
class THaTriggerTime;
class THaVDC;

class THaVDCPlane : public THaSubDetector {

public:

  THaVDCPlane( const char* name="", const char* description="",
	       THaDetectorBase* parent = NULL );
  virtual ~THaVDCPlane();

  virtual void    Clear( Option_t* opt="" );
  virtual Int_t   Decode( const THaEvData& ); // Raw data -> hits
  virtual Int_t   FindClusters();             // Hits -> clusters
  virtual Int_t   FitTracks();                // Clusters -> tracks

  //Get and Set functions
  Int_t          GetNClusters()      const { return fClusters->GetLast()+1; }
  TClonesArray*  GetClusters()       const { return fClusters; }
  THaVDCCluster* GetCluster(Int_t i) const
  { assert( i>=0 && i<GetNClusters() );
    return static_cast<THaVDCCluster*>( fClusters->UncheckedAt(i) ); }

  Int_t          GetNWires()         const { return fWires->GetLast()+1; }
  TClonesArray*  GetWires()          const { return fWires; }
  THaVDCWire*    GetWire(Int_t i)    const
  { assert( i>=0 && i<GetNWires() );
    return static_cast<THaVDCWire*>( fWires->UncheckedAt(i) ); }

  Int_t          GetNHits()          const { return fHits->GetLast()+1; }
  TClonesArray*  GetHits()           const { return fHits; }
  THaVDCHit*     GetHit(Int_t i)     const
  { assert( i>=0 && i<GetNHits() );
    return static_cast<THaVDCHit*>( fHits->UncheckedAt(i) ); }

  Int_t          GetNWiresHit()      const { return fNWiresHit; }
  Int_t          GetNpass()      const { return fNpass; }

  Double_t       GetZ()              const { return fZ; }
  Double_t       GetWBeg()           const { return fWBeg; }
  Double_t       GetWSpac()          const { return fWSpac; }
  Double_t       GetWAngle()         const { return fWAngle; }
  Double_t       GetCosAngle()       const { return fCosAngle; }
  Double_t       GetSinAngle()       const { return fSinAngle; }
  Double_t       GetTDCRes()         const { return fTDCRes; }
  Double_t       GetDriftVel()       const { return fDriftVel; }
  Double_t       GetMinTime()        const { return fMinTime; }
  Double_t       GetMaxTime()        const { return fMaxTime; }
  Double_t       GetMaxTdiff()        const { return fMaxTdiff; }
  Double_t       GetT0Resolution()   const { return fT0Resolution; }

//   Double_t GetT0() const { return fT0; }
//   Int_t GetNumBins() const { return fNumBins; }
//   Float_t *GetTable() const { return fTable; }

protected:

  //Use TClonesArray::GetLast()+1 to get the number of wires, hits, & clusters
  TClonesArray*  fWires;     // Wires
  TClonesArray*  fHits;      // Fired wires
  TClonesArray*  fClusters;  // Clusters

  Int_t fNHits;      // Total number of hits (including multihits)
  Int_t fNWiresHit;  // Number of wires with one or more hits

  // Parameters, read from database
  Int_t fMinClustSize;       // Minimum number of wires needed for a cluster
  Int_t fMaxClustSpan;       // Maximum size of cluster in wire spacings
  Int_t fNMaxGap;            // Max gap in wire numbers in a cluster
  Int_t fMinTime, fMaxTime;  // Min and Max limits of TDC times for clusters
  Int_t fFlags;              // Analysis control flags
  Int_t fNpass;              // Number of hit passes

  Double_t fMinTdiff, fMaxTdiff;  // Min and Max limits of times between wires in cluster

  Double_t fZ;            // Z coordinate of plane in U1 coord sys (m)
  Double_t fWBeg;         // Position of 1-st wire in E-arm coord sys (m)
  Double_t fWSpac;        // Wire spacing and direction (m)
  Double_t fWAngle;       // Angle (rad) between dispersive direction (x) and
                          // normal to wires in dir of increasing wire position
  Double_t fSinAngle;     // sine of fWAngle, for efficiency
  Double_t fCosAngle;     // cosine of fWAngle, for efficiency
  Double_t fTDCRes;       // TDC Resolution ( s / channel)
  Double_t fDriftVel;     // Drift velocity in the wire plane (m/s)

  Double_t fT0Resolution; // (Average) resolution of cluster time offset fit

  // Lookup table parameters
//   Double_t fT0;     // calculated zero time
//   Int_t fNumBins;   // size of lookup table
//   Float_t *fTable;  // time-to-distance lookup table

  VDC::TimeToDistConv* fTTDConv;  // Time-to-distance converter for this plane's wires

  THaVDC* fVDC;           // VDC detector to which this plane belongs

  THaTriggerTime* fglTrg; //! time-offset global variable. Needed at the decode stage

  virtual void  MakePrefix();
  virtual Int_t ReadDatabase( const TDatime& date );
  virtual Int_t DefineVariables( EMode mode = kDefine );

  ClassDef(THaVDCPlane,0)             // VDCPlane class
};

//////////////////////////////////////////////////////////////////////////////

#endif
