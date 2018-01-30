%%%%%%%%%%%%%%%%%%%%%%%%
% Soubor: solution.m
% Projekt: ISS - Projekt 2015 / 16
% Autor: Nikola Valesova
% Datum: 9. 12. 2015
%%%%%%%%%%%%%%%%%%%%%%%%


% vypnuti HW optimalizace
iptsetpref('UseIPPL',false);

% nacteni puvodniho obrazu
img0 = imread('xvales02.bmp');
 
% zaostreni obrazu
mat1 = [-0.5 -0.5 -0.5; -0.5 5.0 -0.5; -0.5 -0.5 -0.5];
img1 = imfilter(img0, mat1);

% ulozeni obrazu po zaostreni 
imwrite(img1, 'step1.bmp');

% otoceni obrazu kolem svisle osy
img2 = fliplr(img1);

% ulozeni obrazu po otoceni 
imwrite(img2, 'step2.bmp');
 
% medianovy filtr
img3 = medfilt2(img2, [5 5]);

% ulozeni obrazu po aplikaci medianoveho filtru 
imwrite(img3, 'step3.bmp');

% rozmazani obrazu
mat2 = [1 1 1 1 1; 1 3 3 3 1; 1 3 9 3 1; 1 3 3 3 1; 1 1 1 1 1] / 49;
img4 = imfilter(img3, mat2);

% ulozeni obrazu po rozmazani
imwrite(img4, 'step4.bmp');

% chyba v obraze
original = fliplr(img0);
original = im2double(original);
edited = im2double(img4);

img_size = size(original);
chyba = 0;

for (i = 1:img_size(1))
    for (j = 1:img_size(2))
        chyba = chyba + (abs(original(i,j) - edited(i,j)));
    end;
end;

chyba = (chyba / (img_size(1) * img_size(2)) * 255)

% roztazeni histogramu
img4_double = im2double(img4);
img_min = min(min(img4_double));
img_max = max(max(img4_double));

img5 = imadjust(img4_double, [img_min, img_max], [0.0 1.0]);
img5 = im2uint8(img5);

% ulozeni obrazu po roztazeni histogramu
imwrite(img5, 'step5.bmp');

% urceni stredni hodnoty a smerodatne odchylky
img5_double = im2double(img5);

mean_no_hist = mean2(img4_double)*255
std_no_hist = std2(img4_double)*255
mean_hist = mean2(img5_double)*255
std_hist = std2(img5_double)*255

% kvantizace obrazu
N = 2;
a = 0;
b = 255;
img_size = size(img5);
img6 = zeros(img_size(1), img_size(2));
img5_double = double(img5);

for (i = 1:img_size(1))
    for (j = 1:img_size(2))
       img6(i,j) = round(((2^N)-1)*(img5_double(i,j)-a)/(b-a))*(b-a)/((2^N)-1) + a;
    end;
end;

img6 = uint8(img6);

% ulozeni obrazu po jeho kvantizaci
imwrite(img6, 'step6.bmp');
