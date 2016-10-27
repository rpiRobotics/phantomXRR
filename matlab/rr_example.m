% Run this AFTER the Robot Raconteur server is started
robot = RobotRaconteur.Connect('tcp://localhost:10001/phantomXRR/phantomXController');

% set robot to starting pose
read_all_joints(robot);
starting_pose = int16( [500;500;500;500;500]  );
robot.setJointPositions(starting_pose);
pause(3) % let robot get into position

% Read all joint positions (should register as ~500 each)
base_pos = read_all_joints(robot);

% Wobble joint positions
a = -30;
b = 30;
for i=1:1:100
    delta_r = int16(((b-a).*rand(5,1) + [a; a; a; a; a]));
    new_pose = int16(delta_r + base_pos);
    robot.setJointPositions(new_pose);
    robot.getJointPosition(int16(1))
    read_all_joints(robot)
    pause(2);
end
    