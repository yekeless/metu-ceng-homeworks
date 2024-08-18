import java.util.ArrayList;

public class CengTreeNodeLeaf extends CengTreeNode
{
    private ArrayList<CengBook> books;
    public CengTreeNodeLeaf nextLeaf;
    public CengTreeNodeLeaf prevLeaf;
    

    public CengTreeNodeLeaf(CengTreeNode parent)
    {
        super(parent);
        nextLeaf = null;
        prevLeaf = null;
        books = new ArrayList<CengBook>();
        type = CengNodeType.Leaf;
    }
    
    public CengTreeNode addBook(CengBook book){
        
        for(int i = 0; i < books.size(); i++){
            if(books.get(i).getBookID() > book.getBookID()){
                books.add(i, book);
                CengTreeNode Root=this;
                
                if(books.size() > 2*order){
                    this.splitLeaf();
                }
                while(Root.getParent()!=null){
                    Root=Root.getParent();
                }
                return Root;
            }
        }
        
        books.add(books.size(), book);
        CengTreeNode Root=this;
        
        if(books.size() > 2*order){
            this.splitLeaf();
        }
        while(Root.getParent()!=null){
            Root=Root.getParent();
        }
        return Root;
        
        
    }
    public void copy_up(int copyUpValue, CengTreeNodeLeaf newLeaf, CengTreeNodeLeaf oldLeaf){
        
        if(this.getParent() != null ){
            int index = ((CengTreeNodeInternal)this.getParent()).addKey(copyUpValue);
            ((CengTreeNodeInternal)this.getParent()).addChildren(index, newLeaf);
            if(((CengTreeNodeInternal)this.getParent()).checkOverflow()){
                ((CengTreeNodeInternal)this.getParent()).splitInternal();
            }
        }
        else{
            CengTreeNodeInternal root = new CengTreeNodeInternal(null);
            ArrayList<Integer> new_keys = new ArrayList<Integer>();
            ArrayList<CengTreeNode> new_children = new ArrayList<CengTreeNode>();
            new_keys.add(copyUpValue);
            new_children.add(oldLeaf);
            new_children.add(newLeaf);
            root.setKeys(new_keys);
            root.setChildren(new_children);
            oldLeaf.setParent(root);
            newLeaf.setParent(root);
        }
       
    }
    public void splitLeaf(){
        CengTreeNodeLeaf newLeaf = new CengTreeNodeLeaf(this.getParent());
        ArrayList<CengBook> newBooks = new ArrayList<CengBook>();
        newBooks.addAll(books.subList(order, books.size()));
        int copyUpValue = newBooks.get(0).getBookID();
        books.subList(order, books.size()).clear();
        newLeaf.books = newBooks;
        newLeaf.nextLeaf = this.nextLeaf;
        newLeaf.prevLeaf = this;
        this.nextLeaf = newLeaf;
        this.copy_up(copyUpValue, newLeaf, this);

    }
    public CengBook searchBook(int bookID){
        for(CengBook book : books){
            if(book.getBookID() == bookID){
                return book;
            }
        }
        return null;
    }
    

    // GUI Methods - Do not modify
    public int bookCount()
    {
        return books.size();
    }
    public Integer bookKeyAtIndex(Integer index)
    {
        if(index >= this.bookCount()) {
            return -1;
        } else {
            CengBook book = this.books.get(index);

            return book.getBookID();
        }
    }
    public void print(int tab){
        
        String tabs="";
        for(int i = 0; i < tab ;i++){
            tabs+="\t";
        }
        System.out.println(tabs+"<data>");
        String book_info;
        for(CengBook book : books){
            book_info=book.fullName();
            String temp=tabs+"<record>"+book_info+"</record>"+"";
            System.out.println(temp);
        }
        System.out.println(tabs+"</data>");
    }
    public void printSearch(int tab, int bookID){
        String tabs="";
        for(int i = 0; i < tab ;i++){
            tabs+="\t";
        }
        
        String book_info;
        for(CengBook book : books){
            if(book.getBookID()==bookID){
                book_info=book.fullName();
                String temp=tabs+"<record>"+book_info+"</record>"+"";
                System.out.println(temp);
            }
        }
        
    }
    
    public boolean is_in(Integer bookID){
        for(int i=0 ; i < books.size() ; i++){
            CengBook book = books.get(i);
            Integer bookID2 = book.getBookID();
            if(bookID2==bookID){
                return true;
            }
        }
        return false;
    }
    public ArrayList<CengTreeNode> searchBook(Integer bookID, ArrayList<CengTreeNode> visited)
    {
        for(int i=0 ; i < books.size() ; i++){
            CengBook book = books.get(i);
            if(book.getBookID()==bookID){
                visited.add(this);
                return visited;
            }
        }
        return null;
    }

    // Extra Functions
}
