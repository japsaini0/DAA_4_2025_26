class Solution {
public:
    int minEatingSpeed(vector<int>& piles, int h)
    {
        sort(piles.begin(),piles.end());

        int n = piles.size();

        int sum = 0;
        int low = 1;
        int high = piles[n-1];
        while(low<high)

        {   
            sum = 0;

            int mid = low+(high-low)/2;

            for(int x : piles)
            {
                sum += ceil((double)x/mid);
            }
            
            if(sum<=h)
            {
                high = mid;
            }

            else
            {
                low = mid +1;
            }
        }
        
        return low;
    }
};