list(APPEND PROJECT_LIBS
	t3ai/t3ai.cpp)

add_executable(${PROJECT_NAME}
	${PROJECT_LIBS}
	main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
	"${PROJECT_LIB_FOLDER}")
