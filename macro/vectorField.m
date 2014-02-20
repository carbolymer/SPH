[X,Y] = meshgrid(-2:.2:2);
% rotation with squeeze
% Zx = Y - X;
% Zy = -X - Y;
% rotation
Zx = -Y;
Zy = X;

figure
hold on
quiver(X,Y,Zx,Zy)
hold off