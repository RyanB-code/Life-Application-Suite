#pragma once

#include <string>
#include <vector>

enum class MuscleGroup{
    BICEPS,
    TRICEPS,
    GRIP_STRENGTH,
    SHOULDERS,
    BACK_UPPER,
    BACK_LOWER,
    CHEST,
    CORE_STABILITY,
    QUADRICEPS,
    HAMSTRINGS,
    CALVES
};

struct Exercise{
    std::string name;
    int sets, reps;
    double weight;
    MuscleGroup primaryTarget;
    MuscleGroup secondaryTarget;
};


class WorkoutProgressTracker{
public:

private:
    std::string m_label;    // What is the overall name of tracker
};