clear all;

filename = 'C:\Users\Adi\Dropbox\my\sem10\EOW\build-Obiekty_dynamiczne-Desktop_Qt_5_4_1_MinGW_32bit-Debug\results.txt';

data = dlmread(filename, ' ');

params = data(1,:);

data = data(2:params(1),:);

# define velues for each axis (in this case random values)
axis_X = data(:, 5);%rand(1, 400);
axis_Y = data(:, 6);%rand(1, 400);
axis_Z = data(:, 7);%rand(1, 400);

# create 360 snapshots (1 per degree)
figure(1);
plot3(axis_X, axis_Y, axis_Z, ".");
grid;
xlabel("Czas regulacji [s]");
ylabel("Przeregulowanie [%]");
zlabel("Calka z odchylki regulacji");