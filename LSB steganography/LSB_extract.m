function [msg_origin] = LSB_extract(name, lsb, color)
% LSB_extract(name, lsb)  LSB in steganography (extract)
% name: the picture's path and name
% lsb: lsb-rightmost LSBs
% color: 1-red, 2-green, 3-blue
% 
% Author: Moming
% 2016-03-17

image = imread(name);

layer = image(:, :, color);
tmp = blanks(0);
n = prod(size(layer));

% if lsb ~= 2, then you need to change something below
for i = 1 : n * lsb / 8
    tmp((i - 1) * 4 + 1 : i * 4) = mod(layer((i - 1) * 4 + 1 : i * 4), 2^lsb); 
    msg((i - 1) * 8 + 1 : i * 8) = dec2bin(tmp((i - 1) * 4 + 1 : i * 4), lsb)';
    msg_origin(i) = bin2dec(msg((i - 1) * 8 + 1 : i * 8));
    if msg_origin(i) == 4  % EOT is the end tag
        break;
    end
end

msg_origin = native2unicode(msg_origin,'UTF-8');
msg_origin = msg_origin(1:end-1);

end