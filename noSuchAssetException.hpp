// noSuchAssetException
class noSuchAssetException{
    public:
        noSuchAssetException()
            :message( "There is no such asset.\n" ){}
        const char *what() const{ return message; }
    private:
        const char *message;
};
