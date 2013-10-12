/*************************************************************************************************
  Description: 
    Storage structure for storing and updating shared memory

    Copyright (c) MWL. All rights reserved.
*************************************************************************************************/

#ifndef _SHARED_MEMORY_HPP_
#define _SHARED_MEMORY_HPP_

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NOTES:
// 
//  -The shared memory variables will be updated once per graphics frame.
//
//  -Each variable comes with a UNIT, RANGE, and UNSET description where applicable.
//     UNITS - Is the numeric form which a variable is stored in (e.g. KPH, Celsius)
//     RANGE - Is the min-max ranges for a variable
//     UNSET - Is the initialised/default/invalid value, depending on the variables usage
//
//  -Constant/unchanging values are included in the data, such as 'maxRPM', 'fuelCapacity' - this is done to allow percentage calculations.
//
//  -Also included are 7 unique enumerated types, to be used against the mentioned flag/state variables
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// *** Types ***

// Header version number to test against
enum
{
  SHARED_MEMORY_VERSION = 3
};

// Maximum allowed length of string
enum
{
  STRING_LENGTH_MAX = 256
};

// Tyres
enum 
{
  TYRE_FRONT_LEFT = 0,
  TYRE_FRONT_RIGHT,
  TYRE_REAR_LEFT,
  TYRE_REAR_RIGHT,
  //--------------
  TYRE_MAX
};

// Vector
enum
{
  VEC_X = 0,
  VEC_Y,
  VEC_Z,
  //-------------
  VEC_MAX
};

// (Type#1) GameState (to be used with 'mGameState')
enum
{
  GAME_EXITED = 0,
  GAME_FRONT_END,
  GAME_INGAME_PLAYING,
  GAME_INGAME_PAUSED,
  //-------------
  GAME_MAX
};

// (Type#2) Session state (to be used with 'mSessionState')
enum
{
  SESSION_INVALID = 0,
  SESSION_PRACTICE,
  SESSION_TEST,
  SESSION_QUALIFY,
  SESSION_RACE,
  SESSION_TIME_ATTACK,
  //-------------
  SESSION_MAX
};

// (Type#3) Terrain Materials (to be used with 'mTerrain')
enum
{
  TERRAIN_ROAD = 0,
  TERRAIN_LOW_GRIP_ROAD,
  TERRAIN_BUMPY_ROAD1,
  TERRAIN_BUMPY_ROAD2,
  TERRAIN_BUMPY_ROAD3,
  TERRAIN_MARBLES,
  TERRAIN_GRASSY_BERMS,
  TERRAIN_GRASS,
  TERRAIN_GRAVEL,
  TERRAIN_BUMPY_GRAVEL,
  TERRAIN_RUMBLE_STRIPS,
  TERRAIN_DRAINS,
  TERRAIN_TYREWALLS,
  TERRAIN_CEMENTWALLS,
  TERRAIN_GUARDRAILS,
  TERRAIN_SAND,
  TERRAIN_BUMPY_SAND,
  TERRAIN_DIRT,
  TERRAIN_BUMPY_DIRT,
  TERRAIN_DIRT_ROAD,
  TERRAIN_BUMPY_DIRT_ROAD,
  TERRAIN_PAVEMENT,
  TERRAIN_DIRT_BANK,
  TERRAIN_WOOD,
  TERRAIN_DRY_VERGE,
  TERRAIN_EXIT_RUMBLE_STRIPS,
  TERRAIN_GRASSCRETE,
  TERRAIN_LONG_GRASS,
  TERRAIN_SLOPE_GRASS,
  TERRAIN_COBBLES,
  TERRAIN_SAND_ROAD,
  TERRAIN_BAKED_CLAY,
  TERRAIN_ASTROTURF,
  TERRAIN_SNOWHALF,
  TERRAIN_SNOWFULL,
  //-------------
  TERRAIN_MAX
};

// (Type#4) Crash Damage State  (to be used with 'mCrashState')
enum
{
  CRASH_DAMAGE_NONE = 0,
  CRASH_DAMAGE_OFFTRACK,
  CRASH_DAMAGE_LARGE_PROP,
  CRASH_DAMAGE_SPINNING,
  CRASH_DAMAGE_ROLLING,
  //-------------
  CRASH_MAX
};

 // (Type#5) Current Sector (to be used with 'mCurrentSector')
enum
{
  SECTOR_INVALID = 0, 
  SECTOR_START,
  SECTOR_SECTOR1,
  SECTOR_SECTOR2,
  SECTOR_FINISH,
  SECTOR_STOP,
  //-------------
  SECTOR_MAX
};

// (Type#6) Car Flags (to be used with 'mCarFlags')
enum
{
  CAR_HEADLIGHT         = (1<<0),
  CAR_ENGINE_ACTIVE     = (1<<1),
  CAR_ENGINE_WARNING    = (1<<2),
  CAR_SPEED_LIMITER     = (1<<3),
  CAR_ABS               = (1<<4),
  CAR_HANDBRAKE         = (1<<5),
};

// (Type#7) Tyre Flags (to be used with 'mTyreFlags')
enum
{
  TYRE_ATTACHED         = (1<<0),
  TYRE_INFLATED         = (1<<1),
  TYRE_IS_ON_GROUND     = (1<<2),
};


// *** Shared Memory ***

typedef struct
{
  // Version Number
  unsigned int mVersion;                           // [ RANGE = 0->... ]

  // Session type
  unsigned int mGameState;                         // [ enum (Type#1) Game state ]
  unsigned int mSessionState;                      // [ enum (Type#2) Session state ]

  // Unfiltered Input
  float mUnfilteredThrottle;                       // [ RANGE = 0.0f->1.0f ]
  float mUnfilteredBrake;                          // [ RANGE = 0.0f->1.0f ]
  float mUnfilteredSteering;                       // [ RANGE = -1.0f->1.0f ]
  float mUnfilteredClutch;                         // [ RANGE = 0.0f->1.0f ]

  // One Time Session
  char mPlayerName[STRING_LENGTH_MAX];             // [ string ]
  char mCarName[STRING_LENGTH_MAX];                // [ string ]
  char mCarClassName[STRING_LENGTH_MAX];           // [ string ]
  char mTrackName[STRING_LENGTH_MAX];              // [ string ]

  // Timing & Scoring
  float mBestLapTime;                              // [ UNITS = seconds ]   [ RANGE = 0.0f->... ]   [ UNSET = -1.0f ]
  float mLastLapTime;                              // [ UNITS = seconds ]   [ RANGE = 0.0f->... ]   [ UNSET = 0.0f ]
  float mCurrentTime;                              // [ UNITS = seconds ]   [ RANGE = 0.0f->... ]   [ UNSET = 0.0f ]
  float mBestSector1Time;                          // [ UNITS = seconds ]   [ RANGE = 0.0f->... ]   [ UNSET = -1.0f ]
  float mCurrentSector1Time;                       // [ UNITS = seconds ]   [ RANGE = 0.0f->... ]   [ UNSET = -1.0f ]
  float mBestSector2Time;                          // [ UNITS = seconds ]   [ RANGE = 0.0f->... ]   [ UNSET = -1.0f ]
  float mCurrentSector2Time;                       // [ UNITS = seconds ]   [ RANGE = 0.0f->... ]   [ UNSET = -1.0f ]
  float mSplitTime;                                // [ UNITS = seconds ]   [ RANGE = 0.0f->... ]   [ UNSET = 0.0f ]
  float mEventTimeRemaining;                       // [ UNITS = milli-seconds ]   [ RANGE = 0.0f->... ]   [ UNSET = -1.0f ]

  // Race Info
  unsigned int mLapsInEvent;                       // [ RANGE = 0->... ]   [ UNSET = 0 ]
  unsigned int mLapsCompleted;                     // [ RANGE = 0->... ]   [ UNSET = 0 ]
  unsigned int mCurrentLap;                        // [ RANGE = 0->... ]   [ UNSET = 0 ]
  unsigned int mCurrentPosition;                   // [ RANGE = 1->... ]   [ UNSET = 0 ]
  unsigned int mNumberOfCars;                      // [ RANGE = 1->... ]   [ UNSET = 0 ]
  unsigned int mCurrentSector;                     // [ enum (Type#5) Current Sector ]

  // Car State
  unsigned int mCarFlags;                          // [ enum (Type#6) Car Flags ]
  float mOilTempCelsius;                           // [ UNITS = Celsius ]   [ UNSET = 0.0f ]
  float mOilPressureKPa;                           // [ UNITS = Kilopascal ]   [ RANGE = 0.0f->... ]   [ UNSET = 0.0f ]
  float mWaterTempCelsius;                         // [ UNITS = Celsius ]   [ UNSET = 0.0f ]
  float mWaterPressureKPa;                         // [ UNITS = Kilopascal ]   [ RANGE = 0.0f->... ]   [ UNSET = 0.0f ]
  float mFuelPressureKPa;                          // [ UNITS = Kilopascal ]   [ RANGE = 0.0f->... ]   [ UNSET = 0.0f ]
  float mFuelLevel;                                // [ RANGE = 0.0f->1.0f ]
  float mFuelCapacity;                             // [ UNITS = Liters ]   [ RANGE = 0.0f->1.0f ]   [ UNSET = 0.0f ]
  float mSpeed;                                    // [ UNITS = Metres per-second ]   [ RANGE = 0.0f->... ]
  float mRpm;                                      // [ UNITS = Revolutions per minute ]   [ RANGE = 0.0f->... ]   [ UNSET = 0.0f ]
  float mMaxRPM;                                   // [ UNITS = Revolutions per minute ]   [ RANGE = 0.0f->... ]   [ UNSET = 0.0f ]
  float mBrake;                                    // [ RANGE = 0.0f->1.0f ]
  float mThrottle;                                 // [ RANGE = 0.0f->1.0f ]
  float mClutch;                                   // [ RANGE = 0.0f->1.0f ]
  float mSteering;                                 // [ RANGE = -1.0f->1.0f ]
  int mGear;                                       // [ RANGE = -1 (Reverse)  0 (Neutral)  1 (Gear 1)  2 (Gear 2)  etc... ]   [ UNSET = 0 (Neutral) ]

  // Motion & Device Related
  float mWorldPosition[VEC_MAX];                   // [ UNITS = World Space  X  Y  Z ]
  float mOrientation[VEC_MAX];                     // [ UNITS = Euler Angles ]
  float mLocalVelocity[VEC_MAX];                   // [ UNITS = Metres per-second ]
  float mWorldVelocity[VEC_MAX];                   // [ UNITS = Metres per-second ]
  float mAngularVelocity[VEC_MAX];                 // [ UNITS = Radians per-second ]
  float mLocalAcceleration[VEC_MAX];               // [ UNITS = Metres per-second ]
  float mWorldAcceleration[VEC_MAX];               // [ UNITS = Metres per-second ]
  float mExtentsCentre[VEC_MAX];                   // [ UNITS = Local Space  X  Y  Z ]

  // Wheels / Tyres
  unsigned int mTyreFlags[TYRE_MAX];               // [ enum (Type#7) Tyre Flags ]
  unsigned int mTerrain[TYRE_MAX];                 // [ enum (Type#3) Terrain Materials ]
  float mTyreY[TYRE_MAX];                          // [ UNITS = Local Space  Y ]
  float mTyreRPS[TYRE_MAX];                        // [ UNITS = Revolutions per second ]
  float mTyreSlipSpeed[TYRE_MAX];                  // [ UNITS = Metres per-second ]
  float mTyreTemp[TYRE_MAX];                       // [ UNITS = Celsius ]   [ UNSET = 0.0f ]
  float mTyreGrip[TYRE_MAX];                       // [ RANGE = 0.0f->1.0f ]
  float mTyreHeightAboveGround[TYRE_MAX];          // [ UNITS = Local Space  Y ]
  float mTyreLateralStiffness[TYRE_MAX];           // [ UNITS = Lateral stiffness coefficient used in tyre deformation ]
  float mTyreWear[TYRE_MAX];                       // [ RANGE = 0.0f->1.0f ]
  float mBrakeDamage[TYRE_MAX];                    // [ RANGE = 0.0f->1.0f ]
  float mSuspensionDamage[TYRE_MAX];               // [ RANGE = 0.0f->1.0f ]

  // Car Damage
  unsigned int mCrashState;                        // [ enum (Type#4) Crash Damage State ]
  float mAeroDamage;                               // [ RANGE = 0.0f->1.0f ]
  float mEngineDamage;                             // [ RANGE = 0.0f->1.0f ]

  // Weather
  float mAmbientTemperature;                       // [ UNITS = Celsius ]   [ UNSET = 25.0f ]
  float mTrackTemperature;                         // [ UNITS = Celsius ]   [ UNSET = 30.0f ]
  float mRainDensity;                              // [ UNITS = How much rain will fall ]   [ RANGE = 0.0f->1.0f ]
  float mWindSpeed;                                // [ RANGE = 0.0f->100.0f ]   [ UNSET = 2.0f ]
  float mWindDirectionX;                           // [ UNITS = Normalised Vector X ]
  float mWindDirectionY;                           // [ UNITS = Normalised Vector Y ]
  float mCloudBrightness;                          // [ RANGE = 0.0f->... ]
} SharedMemory;


#endif  // _SHARED_MEMORY_HPP_
