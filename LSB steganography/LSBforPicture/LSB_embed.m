function [] = LSB_embed(host, data)
% [] = LSB_embed(host, data)
% host: the host picture's path and name
% data: the data picture's path and name
% LSB in steganography (embed)
%
% Author: Moming
% 2016-03-21

lsb = 3;
host_image = imread(host);
data_image = imread(data);

len = length(dec2bin(max(size(data_image))));
len = dec2bin(len, 9);  % the length of max(height, width) < 2^999
image_info = dec2bin(size(data_image));
info = strjoin(cellstr(image_info)', '');

info_len = length(info) / lsb;

% is host picture big enough?
if numel(data_image) * 3 + info_len + 1 > numel(host_image)
    warning('The host picture is too small to hide the data picture!');
    return;
end

msg_bin = dec2bin(data_image, 8);  % convert to binary
msg = blanks(9);
for i = 1 : size(msg_bin, 1)
    msg(i, :) = strcat(msg_bin(i, :), char(mod(i, 2) + '0'));
end
msg = strjoin(cellstr(msg)', '');
% change the last bit as the data end tag
msg(end) = char(mod(size(msg_bin, 1) + 1, 2) + '0');  

tmp_len = blanks(3);
for i = 1 : 3
    % convert to decimal (len)
    tmp_len(i) = char(bin2dec(len((i - 1) * lsb + 1 : i * lsb)) + '0'); 
end

tmp_info = blanks(info_len);
for i = 1 : info_len
    % convert to decimal (info)
    tmp_info(i) = char(bin2dec(info((i - 1) * lsb + 1 : i * lsb)) + '0');  
end

data_len = length(msg) / lsb;
tmp_data = blanks(data_len);
for i = 1 : data_len
    % convert to decimal (data)
    tmp_data(i) = char(bin2dec(msg((i - 1) * lsb + 1 : i * lsb)) + '0');  
end

result = host_image;
rgb = 1;
[len_R, len_G, len_B] = size(result);

% hide len
for i = 1 : 3
    result(len_R, len_G, i) = result(len_R, len_G, i) - ...
        mod(result(len_R, len_G, i), 2^lsb) + double(tmp_len(i) - '0');
end

% hide info
for R = len_R : -1 : 1
    for G = len_G : -1 : 1
        if R == len_R && G == len_G
            continue;
        end
        for B = len_B : -1 : 1
            if rgb <= info_len
                result(R, G, B) = result(R, G, B) - mod(result(R, G, B),...
                    2^lsb) + double(tmp_info(rgb) - '0');
                rgb = rgb + 1;
            end
        end
    end
end

% hide data
rgb = 1;
for R = 1 : len_R
    for G = 1 : len_G
        for B = 1 : len_B
            if rgb <= data_len
                % only to be consistent with front: '0'
                result(R, G, B) = result(R, G, B) - mod(result(R, G, B),...
                    2^lsb) + double(tmp_data(rgb) - '0');
                rgb = rgb + 1;
            end
        end
    end
end

imshow(result);
imwrite(result, 'result.png');  % do not use jpg

end
