#ifndef WAYROUND_I2P_20250205_113427_112148
#define WAYROUND_I2P_20250205_113427_112148

#include <sigc++/sigc++.h>

#include "signal.hpp"

namespace wayround_i2p::ccutils::signal
{

template <class>
class SlotSigCCompat;

template <class RetT, class... Args>
class SlotSigCCompat<RetT(Args...)>;

template <class RetT, class... Args>
class SlotSigCCompat<RetT(Args...)>
    : public SlotC<RetT(Args...)>,
      public sigc::trackable
{
  public:
    SlotSigCCompat(std::function<RetT(Args...)> fun = nullptr) :
        SlotC<RetT(Args...)>(fun)
    {
    }

    ~SlotSigCCompat()
    {
    }

    RetT on_emission(Args... args)
    {
        auto x = dynamic_cast<Slot_shared<RetT(Args...)> *>(this);
        if (!x)
        {
            // todo: better error
            throw "!x";
        }

        return (*x)->on_emission(args...);
    }

    sigc::slot<RetT(Args...)> make_sigc_slot()
    {
        return sigc::mem_fun(*this, &SlotSigCCompat<RetT(Args...)>::on_emission);
    }

    operator sigc::slot<RetT(Args...)>()
    {
        return make_sigc_slot();
    }
};
} // namespace wayround_i2p::ccedit

#endif
