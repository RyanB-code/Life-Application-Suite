#pragma once

#include <string>
#include <vector>

// Specify which muscle groups are targeted for a specific exercise
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

// Classification of a specfic exercise
enum class ExerciseClassification{
    CARDIO_AEROBIC,
    CARDIO_ANAEROBIC,
    CARDIO_GLYCOLYTIC,
    RESISTANCE
};

// Used to help organize a Workout set
enum class WorkoutTags{
    PUSH,
    PULL,
    STABILITY,
    CALISTHENICS,
    INTERVAL,
    LONG_SLOW_DISTANCE,
    AMRAP,
    TABATA,
};


struct Exercise {
    std::string                 m_name;
    std::string                 m_description;
    std::vector<MuscleGroup>    m_targetedMuscleGroups;        // Add these in order of precedence
    ExerciseClassification      m_classification;              
};


class WorkoutProgressTracker{
public:

private:
   
};