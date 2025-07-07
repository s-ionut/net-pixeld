#include "Application.hpp"

int gScreenWidth = 800;
int gScreenHeight = 450;

int main()
{
    Application app(gScreenWidth, gScreenHeight, "net-pixeld-client");

    app.Run();

    return 0;
}