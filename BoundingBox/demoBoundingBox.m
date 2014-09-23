% Test the functions in this toolbox and show the results.
%
%   Author: Ying Xiong
%   Created: Sep 11, 2012

overall_bbox = [-10 20 30 60];
PlotBBox(overall_bbox); hold on;

existing_bboxes = [3 4 35 36
                   -5 0 50 55];
PlotBBox(existing_bboxes, 'r'); hold on;

new_bbox_size = [3 5];
new_bboxes = GenerateNonOverlapBBoxes(overall_bbox, new_bbox_size, ...
                                      existing_bboxes, -1, 100);

PlotBBox(new_bboxes);
