#!/usr/bin/env python3
"""
Focus Score Calculation Algorithm
Analyzes sensor data to determine student engagement level
"""

import json
import sys
import numpy as np
from datetime import datetime

def calculate_focus_score(sensor_data):
    """
    Calculate focus score based on multiple sensor inputs
    
    Parameters:
    - heart_rate: Heart rate variability (lower variability = better focus)
    - movement: Accelerometer data (less movement = better focus)
    - interaction: Device interaction frequency (less = better focus)
    
    Returns:
    - focus_score: 0-100 score indicating engagement level
    """
    
    # Extract sensor data
    heart_rate_avg = sensor_data.get('heartRateAvg', 75)
    movement_count = sensor_data.get('movementCount', 0)
    interaction_count = sensor_data.get('interactionCount', 0)
    duration_minutes = sensor_data.get('durationMinutes', 45)
    
    # Normalize values (0-1 scale)
    # Heart rate: 60-80 is optimal, outside this range reduces score
    hr_score = 1.0
    if heart_rate_avg < 60:
        hr_score = max(0, 1 - (60 - heart_rate_avg) / 20)
    elif heart_rate_avg > 80:
        hr_score = max(0, 1 - (heart_rate_avg - 80) / 30)
    
    # Movement: Less than 5 movements per 10 minutes is good
    expected_movement = (duration_minutes / 10) * 5
    movement_score = max(0, 1 - (movement_count / (expected_movement * 2)))
    
    # Interaction: Less than 3 interactions per 10 minutes is good
    expected_interaction = (duration_minutes / 10) * 3
    interaction_score = max(0, 1 - (interaction_count / (expected_interaction * 2)))
    
    # Weighted average
    weights = {
        'heart_rate': 0.25,
        'movement': 0.40,
        'interaction': 0.35
    }
    
    focus_score = (
        hr_score * weights['heart_rate'] +
        movement_score * weights['movement'] +
        interaction_score * weights['interaction']
    ) * 100
    
    # Calculate distraction count
    distraction_threshold = 10  # movements
    distraction_count = max(0, (movement_count - expected_movement) / distraction_threshold)
    
    return {
        'focusScore': round(focus_score, 2),
        'distractionCount': int(distraction_count),
        'breakdown': {
            'heartRateScore': round(hr_score * 100, 2),
            'movementScore': round(movement_score * 100, 2),
            'interactionScore': round(interaction_score * 100, 2)
        }
    }

def main():
    try:
        # Read sensor data from command line argument
        sensor_data_json = sys.argv[1]
        sensor_data = json.loads(sensor_data_json)
        
        # Calculate focus score
        result = calculate_focus_score(sensor_data)
        
        # Output result as JSON
        print(json.dumps(result))
        sys.exit(0)
        
    except Exception as e:
        error_result = {
            'error': str(e),
            'focusScore': 0
        }
        print(json.dumps(error_result))
        sys.exit(1)

if __name__ == '__main__':
    main()
