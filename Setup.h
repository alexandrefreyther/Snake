class Setup
{   
    public :

    void backgroundSetup(const int nx, const int ny, int bg[]); // va modifier le background initial
    void setupSnake(int snake[], int snake_len);

    private : 
    const int m_nx;
    const int m_ny;
    int m_bg[];
    int m_snake_len;
    int m_snake;

}