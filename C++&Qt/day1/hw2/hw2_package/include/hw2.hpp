class hw2
{
    typedef struct _Coordinate
    {
        int x;
        int y;
    } Coordinate;

  public:
    Coordinate *clist;
    int coordinate_count;
    float dist;
    float d_min, d_max = 0;
    int max_idx_1, max_idx_2;
    int min_idx_1, min_idx_2;
    void newList();
    void gainMaxMin();
    void deleteList();
};
