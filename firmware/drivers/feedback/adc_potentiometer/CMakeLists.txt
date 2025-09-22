if(CONFIG_OMSL_FEEDBACK_ADC_POTENTIOMETER)
    target_sources(app PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/adc_potentiometer_driver.cpp
    )
    target_include_directories(app PRIVATE ${CMAKE_CURRENT_LIST_DIR})
endif()
