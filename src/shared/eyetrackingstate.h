#ifndef EYETRACKINGSTATE_H
#define EYETRACKINGSTATE_H
#include "string"

namespace ass {
    class EyeTrackingState
    {
    public:
        EyeTrackingState();
        int GetEyeTrackingState();
        std::string GetEyeTrackingStateString();
        void SetEyeTrackingState(int state);
        static int state;
    };

}
#endif // EYETRACKINGSTATE_H
