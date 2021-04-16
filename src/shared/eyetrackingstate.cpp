#include "eyetrackingstate.h"

namespace ass {
    EyeTrackingState::EyeTrackingState()
    {
    }

    int EyeTrackingState::GetEyeTrackingState() {
        return ass::EyeTrackingState::state;
    }

    std::string EyeTrackingState::GetEyeTrackingStateString() {
        switch (ass::EyeTrackingState::state) {
            case (-1):
                return "Left";
            case (0):
                return "Center";
            case (1):
                return "Right";
            default:
                return "Undefined";
        }
    }

    void EyeTrackingState::SetEyeTrackingState(int state) {
        ass::EyeTrackingState::state = state;
    }
}

// Initalize state to the shared sentinel flag
int ass::EyeTrackingState::state = 99;
