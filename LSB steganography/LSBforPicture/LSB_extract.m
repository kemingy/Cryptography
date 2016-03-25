function [] = LSB_extract(name)
% LSB_extract(name)
% name: the picture's path and name
% LSB in steganography (extract)
% 
% Author: Moming
% 2016-03-21

image = imread(name);

lsb = 3;
[len_R, len_G, len_B] = size(image);
flag = char('0');

tmp_len = blanks(3);
for i = 1 : 3
    tmp_len = strcat(tmp_len, mod(image(len_R, len_G, i), 2^lsb) + '0');
end
len = sum(bin2dec(strjoin(cellstr(dec2bin(tmp_len - '0', 3))', '')));

% get the size of hide picture
index = 1;
tmp_info = blanks(len);
for R = len_R : -1 : 1
    if index > len
        break;
    end
    for G = len_G : -1 : 1
        if index > len
            break;
        end
        if R == len_R && G == len_G
            continue;
        end
        for B = len_B : -1 : 1
            if index > len
                break;
            end
            tmp_info(index) = mod(image(R, G, B), 2^lsb) + '0';
            index = index + 1;
        end
    end
end
cmd = strjoin(cellstr(dec2bin(tmp_info - '0'))', '');
image_size = zeros(1, 3);
for i = 1 : 3
    image_size(i) = bin2dec(cmd((i - 1) * len + 1 : i * len));
end

% get the hide picture
index = 1;
picture = zeros(image_size);
for R = 1 : len_R
    for G = 1 : len_G
        tmp = blanks(0);
        for B = 1 : len_B
             % '0' is useful!!! Placeholder...
            tmp = strcat(tmp, mod(image(R, G, B), 2^lsb) + '0'); 
        end
        tmp_bin = dec2bin(tmp - '0', 3)';
        picture(index) = bin2dec(tmp_bin(1 : 8));
        if flag + tmp_bin(9) ~= 97  % '0'/'1' is the end tag
            % recover the picture
            picture = uint8(picture);  % !!! very important !!!
            imwrite(picture, 'recover.png');
            imshow(picture);
            return;
        end
        index = index + 1;
        flag = tmp_bin(9);
    end
end

end