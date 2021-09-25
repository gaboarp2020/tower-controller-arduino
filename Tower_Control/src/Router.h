#ifndef Router_h
#define Router_h

// PAGES
#define PAGE_BOOT "boot"
#define PAGE_ELEVATE_UP "elevateUp"
#define PAGE_ELEVATE_DOWN "elevateDown"
#define PAGE_INCLINATE_UP "inclinateUp"
#define PAGE_INCLINATE_DOWN "inclinateDown"
#define PAGE_MAIN "main"
#define PAGE_STOP "stop"

class Router
{
public:
    Router();
    void goTo(String page);
    String current();

private:
    String _currentPage;
    // Internal Methods
    bool isValidPage(String page);
};

Router::Router()
{
    this->goTo(PAGE_BOOT);
}

void Router::goTo(String page)
{
    if (!this->isValidPage(page))
    {
        _currentPage = PAGE_MAIN;

        return;
    }

    _currentPage = page;
}

String Router::current()
{
    return _currentPage;
}

bool Router::isValidPage(String page)
{
    return page == PAGE_BOOT ||
           page == PAGE_ELEVATE_UP ||
           page == PAGE_ELEVATE_DOWN ||
           page == PAGE_INCLINATE_UP ||
           page == PAGE_INCLINATE_DOWN ||
           page == PAGE_STOP ||
           page == PAGE_MAIN;
}

#endif
