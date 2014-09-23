function new_bboxes = GenerateNonOverlapBBoxes(overall_bbox, new_bbox_size, ...
                          existing_bboxes, num_new_bboxes, max_iter)

% Generating a set of non-overlapping bounding boxes. Current implementation
% use random sample technique.
%
% USAGE:
%   new_bboxes = GenerateNonOverlapBBoxes(overall_bbox, new_bbox_size, ...
%                    existing_bboxes, num_new_bboxes, max_iter)
%
% INPUT:
%   overall_bbox: 1x4 array describing the overall bounding box that all
%                 results lie in.
%   new_bbox_size: 2x1 vector describing the new bounding box size.
%   existing_bboxes: Nx4 matrix, each line is an existing bounding box that the
%                    generated bounding box will not overlap.
%   num_new_bboxes: number of new bounding boxes, if -1, generate as many
%                   bounding boxes as possible.
%   max_iter: max number if iteration for sampling. If this number is reached
%             when the num_new_bboxes is still not fulfilled, the function will
%             still exit.
% OUTPUT:
%   new_bboxes: Mx4 matrix, each row describing a bounding box, no two bounding
%               boxes (new and existing) will overlap with each other.
%
%   Author: Ying Xiong
%   Created: Sep 11, 2012

assert(size(existing_bboxes, 2) == 4);
num_exist = size(existing_bboxes, 1);

% Possible bottom-left corners areas to sample from.
PossibleBottomLeft = overall_bbox - [0 new_bbox_size(1) 0 new_bbox_size(2)];

% Forbidden zones for bottom-left corner.
ForbidBottomLeft = zeros(num_exist, 4);
for i = 1:size(existing_bboxes)
  bbox = existing_bboxes(i, :);
  ForbidBottomLeft(i, :) = bbox - [new_bbox_size(1) 0 new_bbox_size(2) 0];
end

% Randomly sample from PossibleBottomLeft region, and add to new_bboxes
% if that location is not forbidden.
if (num_new_bboxes > 0)
  new_bboxes = zeros(num_new_bboxes, 4);
else
  new_bboxes = [];
end
num_cur_bboxes = 0;
for i = 1:max_iter
  % Sample.
  x = PossibleBottomLeft(1) + rand(1) * ...
          (PossibleBottomLeft(2) - PossibleBottomLeft(1));
  y = PossibleBottomLeft(3) + rand(1) * ...
          (PossibleBottomLeft(4) - PossibleBottomLeft(3));
  % Check whether forbidden.
  forbid = false;
  for j = 1:size(ForbidBottomLeft)
    if (BBoxContains(ForbidBottomLeft(j, :), [x,y]))
      forbid = true;
    end
  end
  if (forbid)
    continue;
  end
  % Add to new bounding boxes.
  num_cur_bboxes = num_cur_bboxes + 1;
  if (num_new_bboxes > 0)
    new_bboxes(num_cur_bboxes, :) = ...
                           [x, x+new_bbox_size(1), y, y+new_bbox_size(2)];
    ForbidBottomLeft(end+1, :) = new_bboxes(num_cur_bboxes, :) - ...
                                    [new_bbox_size(1) 0 new_bbox_size(2) 0];
    if (num_cur_bboxes == num_new_bboxes)
      break;
    end
  else
    new_bboxes(end+1, :) = [x, x+new_bbox_size(1), y, y+new_bbox_size(2)];
    ForbidBottomLeft(end+1, :) = new_bboxes(end, :) - ...
                                    [new_bbox_size(1) 0 new_bbox_size(2) 0];
  end
end

if (num_cur_bboxes < num_new_bboxes)
  new_bboxes = new_bboxes(1:num_cur_bboxes, :);
end
