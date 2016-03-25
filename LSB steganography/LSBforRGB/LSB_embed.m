function [] = LSB_embed(name, message)
% LSBembed(name, message, lsb)
% name: the picture's path and name
% message: the data you want to hide in the picture
% LSB in steganography (embed)
% 
% Author: Moming
% 2016-03-20

lsb = 3;
image = imread(name);
msg_origin = unicode2native(message, 'UTF-8');  % UTF-8 encode
msg_bin = dec2bin(msg_origin, 8);  % convert to binary
msg = blanks(9);
for i = 1 : size(msg_bin, 1)
    msg(i, :) = strcat(msg_bin(i, :), char(mod(i, 2) + '0'));
end
msg = strjoin(cellstr(msg)','');
msg(end) = char(mod(size(msg_bin, 1) + 1, 2) + '0');  % change the last bit as the end tag

len = length(msg) / lsb;
tmp = blanks(len);
for i = 1 : len
    tmp(i) = char(bin2dec(msg((i - 1) * lsb + 1 : i * lsb)) + '0');  % convert to decimal
end

% use RGB
result = image;
rgb = 1;
[len_R, len_G, len_B] = size(result);

for R = 1 : len_R
    for G = 1 : len_G
        for B = 1 : len_B
            if rgb <= len
                % only to be consistent with front: '0'
                result(R, G, B) = result(R, G, B) - mod(result(R, G, B), 2^lsb) + double(tmp(rgb) - '0');
                rgb = rgb + 1;
            end
        end
    end
end

imshow(result);
imwrite(result, 'result.png');  % jpg would lose some information

end