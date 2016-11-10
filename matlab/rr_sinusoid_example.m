% Run this AFTER the Robot Raconteur server is started
robot = RobotRaconteur.Connect('tcp://localhost:10001/phantomXRR/phantomXController');
base_pose = int16( [500;500;500;500;500] );
robot.setJointPositions(base_pose);
pause(1);

disp 'Going into twitch loop'
pi_inc = pi / 10;
for i=1:1:100
    delta_r = 30*int16( sin( ones(1,5) * pi_inc*i) );
    new_pose = int16(delta_r' + base_pose);
    robot.setJointPositions(new_pose);
    robot.getJointPositions()
    pause(.5);
end