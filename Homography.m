% Measured touch coordinates
pts_touch = [1421, 1611 %BL
             1659, 1608; %TL
             1987, 2055; %TR
             1371, 1870]; %BR

  % pts_touch = [935, 605; %TL
  %            336, 1262 ]; %BR

%Target screen coordinates
pts_screen = [ 10, 10;
              790, 10;
              790, 470;
              10, 470 ];

% pts_screen = [ 790, 10;
%                10, 470 ];

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
