Build and run:

cd ~/rviz_mod && source ~/ws_tomo/install/setup.bash && catkin build

source devel/setup.bash && rosrun rviz quick_render_panel_test

cd ~/rviz_mod && source ~/ws_tomo/install/setup.bash && catkin build && source devel/setup.bash && rosrun rviz quick_render_panel_test

cd ~/rviz_mod && source ~/ws_tomo/install/setup.bash && catkin build && source devel/setup.bash && roslaunch tomo_ui tomo_ui.launch

Debug:
gdb --args devel/lib/rviz/quick_render_panel_test
(gdb) run
(gdb) bt
valgrind --leak-check=full devel/lib/rviz/quick_render_panel_test
sudo apt-get install valgrind

Mod from Rviz 1.13.30 and Moveit 1.0.11
