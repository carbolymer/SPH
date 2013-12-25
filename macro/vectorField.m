[X,Y] = meshgrid(-2:.2:2);
Zx = Y - X;
Zy = -X - Y;

figure
hold on
quiver(X,Y,Zx,Zy)
hold off