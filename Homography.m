clc;clear all; 
% Measured touch coordinates
pts_touch = [ 2296,1508;
 2271,1863;
 2336,2294;
 1829,1719;
 1819,1983;
 1850,2307;
 1246,1861;
 1226,2122;
 1121,2412];



%Target screen coordinates
pts_screen = [750, 50;     %TL
              750, 240;     %TM
              750, 430;     %TR
              400, 50;      %ML
              400, 240;     %MM
              400, 430;     %MR
              50, 50;       %BL
              50, 240;      %BM
              50, 430];     %BR



% Solve for homography
tform = fitgeotform2d(pts_touch, pts_screen, 'projective');
H = tform.A;

% Display the homography matrix
disp('Homography matrix H:');
disp(H);

% Test transforming a sample point
test_point_touch = [1000; 2000; 1];
mapped_point = H * test_point_touch;
mapped_point = mapped_point ./ mapped_point(3);

fprintf('Mapped point: (%.2f, %.2f)\n', mapped_point(1), mapped_point(2));
