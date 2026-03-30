add_executable(MibletsGame
  Main.cpp
  MibletsApp.cpp
  TurnManager.cpp
)

target_link_libraries(MibletsGame PRIVATE MibletsEngine)
