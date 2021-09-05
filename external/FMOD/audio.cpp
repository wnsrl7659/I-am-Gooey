#include <iostream>
#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <fmod_errors.h>

#include "trace.h"

namespace Audio
{
  #define FBank FMOD::Studio::Bank

  static FMOD::Studio::System * system = NULL; // fmod studio system
  static FMOD::System * lowLevelSystem = NULL; // fmod low level system
  static FMOD_RESULT result; // result for error checking
  static std::ostringstream e; // string stream for trace messaging

  static FBank * masterBank = NULL; // master soundbank
  static FBank * stringsBank = NULL; // strings soundbank
  static FBank * ambBank = NULL; // ambience soundbank
  static FBank * musicBank = NULL; // music soundbank
  static FBank * sfxBank = NULL; // sfx soundbank

  static void ERRCHECK(FMOD_RESULT r)
  {
    if (r != FMOD_OK)
    {
      e << "FMOD error! (" << result << ") " << FMOD_ErrorString(r);
      Trace::Message(&e);
      exit(-1);
    }
  }

  void Init(void)
  {
    Trace::Report("Audio: Init");
    Trace::Report("Audio: FMOD System Create");
    result = FMOD::Studio::System::create(&system); // create the fmod system
    ERRCHECK(result);

    // get low level system as well
    ERRCHECK(system->getLowLevelSystem(&lowLevelSystem));
    // init both systems
    Trace::Report("Audio: FMOD Systems Init");
    result = system->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
    ERRCHECK(result);

    // load soundbanks
    ERRCHECK(system->loadBankFile("./studio/Build/desktop/Master Bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank)); // master bank
    ERRCHECK(system->loadBankFile("./studio/Build/desktop/Master Bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank)); // strings bank
    ERRCHECK(system->loadBankFile("./studio/Build/desktop/Ambience.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &ambBank)); // ambience bank
    ERRCHECK(system->loadBankFile("./studio/Build/desktop/Music.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &musicBank)); //music bank
    ERRCHECK(system->loadBankFile("./studio/Build/desktop/SFX.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &sfxBank)); // sfx bank
  }

  void Update(float dt)
  {
    static_cast<void>(dt);
    result = system->update();
    ERRCHECK(result);
  }

  void Shutdown(void)
  {
    result = system->release();
    ERRCHECK(result);
  }
}