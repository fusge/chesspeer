install(
    TARGETS Athena_exe
    RUNTIME COMPONENT Athena_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
