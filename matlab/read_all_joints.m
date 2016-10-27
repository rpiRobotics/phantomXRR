function [ joint_readings ] = read_all_joints(robot)
%read_all_joints returns all joints associated with the passed robot
%raconteur object

    % Read all joint positions
    joint_readings = int16([-1; -1; -1; -1; -1]);
    for i=1:1:5
        joint_readings(i) = robot.getJointPosition(int16(i));
    end
end

