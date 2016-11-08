% Run this AFTER the Robot Raconteur server is started
robot = RobotRaconteur.Connect('tcp://localhost:10001/phantomXRR/phantomXController');
base_pose = int16( [400;500;500;500;500] );
robot.setJointPositions(base_pose);
pause(1)

disp 'Going into twitch loop'
a = -30; b = 30;
for i=1:1:100
    delta_r = int16(((b-a).*rand(5,1) + [a; a; a; a; a]));
    new_pose = int16(delta_r + base_pose);
    robot.setJointPositions(new_pose);
    pause(.1);
    robot.getJointPositions()
    pause(1);
end