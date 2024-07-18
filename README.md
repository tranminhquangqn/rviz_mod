build: cd ~/rviz_mod && source ~/ws_tomo/install/setup.bash && catkin build
run: source devel/setup.bash && rosrun rviz quick_render_panel_test
Debug:
gdb --args devel/lib/rviz/quick_render_panel_test
(gdb) run
(gdb) backtrace
valgrind --leak-check=full devel/lib/rviz/quick_render_panel_test

sudo apt-get install valgrind
