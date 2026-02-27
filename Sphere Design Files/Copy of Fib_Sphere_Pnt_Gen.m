% Name: Lenny Marron
% File Name: Fib_Sphere_Pnt_Gen.m
% Date: 9/19/25
% Description: This is a script generates 
% Fibonacci Sphere Points and saves them on a CSV file


% ****************************************************
clear, clc
% ****************************************************

% Define the number of points and the sphere's radius
num_points = 64;  % sensor number
radius = 6.5;     % unitless value
filename = 'Fibonacci_sphere_points.csv';

% The golden angle in radians
golden_angle = pi * (3 - sqrt(5));

% Pre-allocate arrays for efficiency
points = zeros(num_points, 3);

for i = 1:num_points
    % Calculates the relative y-coordinate
    y = 1 - (i - 1) / (num_points - 1) * 2;
    % Calculates the radius of the circle at this y-coordinate
    current_radius = sqrt(1 - y^2);
    % Calculates the golden angle increment
    theta = golden_angle * (i - 1);

    % Converts from spherical to Cartesian coordinates
    x = cos(theta) * current_radius;
    z = sin(theta) * current_radius;

    % Stores the point, scaled by the desired radius
    points(i, :) = [x, y, z] * radius;
end


% Exports the points to CSV file
writematrix(points, filename);