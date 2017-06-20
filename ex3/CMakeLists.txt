#############################################################
### YOU SHOULD EDIT THE BELOW WITH YOUR DETAILS AND FILES ###
#############################################################

set(user1 meitalswissa)
set(user2 razramon)
set(id1 "302461876")
set(id2 "315474197")

# All source files needed to compile your exe
set (EXE_SOURCE_FILES
	submarinesGame.h
	submarinesGame.cpp
	IBattleshipGameAlgo.h
	BoardCreator.h
	BoardCreator.cpp
	Sizes.h
	Ship.h
	Ship.cpp
	Utilities.h
	Utilities.cpp
	Logger.h
	Logger.cpp
	Exception.h
	Exception.cpp
	PlayerInfo.h
	PlayerInfo.cpp
	Game.h
	Game.cpp
	GameInfo.h
	GameInfo.cpp
	GameManager.h
	GameManager.cpp
	GameBasicData.h
	GameBasicData.cpp
	Board.h
	Board.cpp
	Macros.h
)
# All source files needed to compile your smart algorithm's .dll
set (SMART_ALGO_SOURCE_FILES
	IBattleshipGameAlgo.h
	BoardCreator.h
	BoardCreator.cpp
	Sizes.h
	Ship.h
	Ship.cpp
	Utilities.h
	Utilities.cpp
	Logger.h
	Logger.cpp
	Exception.h
	Exception.cpp
	DllSmartAlgo.h
	DllSmartAlgo.cpp
	Macros.h
)

##################################
### DO NOT CHANGE FROM HERE ON ###
##################################

message("user: ${user1}, id: ${id1}")

add_definitions(-DUNICODE -D_UNICODE)

set(EX ex3)

# Set exe build parameters
set(EXE_NAME "${EX}.${user1}")
add_executable( ${EXE_NAME}	${EXE_SOURCE_FILES})
set_property(TARGET ${EXE_NAME} PROPERTY FOLDER "${EX}/${user1}")
set_property(TARGET ${EXE_NAME} PROPERTY OUTPUT_NAME "${user1}")

# Set smart algorithm's .dll build parameters
set(ALG3_NAME "${EX}.${user1}.smart")
add_library(${ALG3_NAME} SHARED ${SMART_ALGO_SOURCE_FILES})
set_property(TARGET ${ALG3_NAME} PROPERTY FOLDER "${EX}/${user1}")
set_property(TARGET ${ALG3_NAME} PROPERTY OUTPUT_NAME "${user1}.smart")
target_compile_definitions(${ALG3_NAME} PRIVATE -DALGO_EXPORTS) # preprocessor flag saying we are the one compiling the header now