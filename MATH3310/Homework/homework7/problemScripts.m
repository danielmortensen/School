% define adjacency matrix
A =  [0 0 0 0 0 0 0 1 1 1;...
      0 0 0 0 0 1 1 0 0 1;...
      0 0 0 0 1 0 1 0 1 0;...
      0 0 0 0 1 1 0 1 0 0;...
      0 0 1 1 0 0 0 0 0 1;...
      0 1 0 1 0 0 0 0 1 0;...
      0 1 1 0 0 0 0 1 0 0;...
      1 0 0 1 0 0 1 0 0 0;...
      1 0 1 0 0 1 0 0 0 0;...
      1 1 0 0 1 0 0 0 0 0];


nTreeA = computeNumberOfSpanningTrees(A);

% compute number of trees for G'
APrime = A;
APrime(1,end) = 0;
APrime(end,1) = 0;
nTreeAPrime = computeNumberOfSpanningTrees(APrime);

APrime2First = APrime;
APrime2First(1,end-1) = 0;
APrime2First(end-1,1) = 0;

nTreeAPrime2First = computeNumberOfSpanningTrees(APrime2First);

APrime2Second = APrime;
APrime2Second(2,6) = 0;
APrime2Second(6,2) = 0;

nTreeAPrime2Second = computeNumberOfSpanningTrees(APrime2Second);

% number of steps of length 1 or 2
A2 = A^2 + A;

% number of 3-cycles
A = [0 1 0 0 1 0 1 1 1 1 0;...
     1 0 1 0 0 1 0 1 0 0 0;...
     0 1 0 1 0 0 1 0 1 0 0;...
     0 0 1 0 1 0 0 1 0 1 0;...
     1 0 0 1 0 1 0 0 1 0 0;...
     0 1 0 0 1 0 0 0 0 0 1;...
     1 0 1 0 0 0 0 0 0 0 1;...
     1 1 0 1 0 0 0 0 0 0 1;...
     1 0 1 0 1 0 0 0 0 0 1;...
     1 0 0 1 0 0 0 0 0 0 1;...
     0 0 0 0 0 1 1 1 1 1 0];
n3Cycle = trace(A^3)/6;





function nTree = computeNumberOfSpanningTrees(A)

% compute degree of each vertex
D = diag(sum(A,1));

% compute laplacian
L = D - A;

% compute number of spanning trees
L1 = L(:,2:end);
L1 = L1(2:end,:);
nTree = det(L1);
end