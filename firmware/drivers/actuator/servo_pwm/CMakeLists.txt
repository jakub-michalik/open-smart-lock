if(CONFIG_OMSL_ACTUATOR_SERVO_PWM)
    target_sources(app PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/servo_pwm_driver.cpp
    )
    target_include_directories(app PRIVATE ${CMAKE_CURRENT_LIST_DIR})
endif()
