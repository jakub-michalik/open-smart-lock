if(CONFIG_OMSL_POWER_GPIO_GATE)
    target_sources(app PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/gpio_gate_driver.cpp
    )
    target_include_directories(app PRIVATE ${CMAKE_CURRENT_LIST_DIR})
endif()
