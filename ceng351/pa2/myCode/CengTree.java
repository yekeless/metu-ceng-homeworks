import java.util.ArrayList;

public class CengTree
{
    public CengTreeNode root;
    // Any extra attributes...

    public CengTree(Integer order)
    {
        CengTreeNode.order = order;
        // TODO: Initialize the class
        root = new CengTreeNodeLeaf(null);
    }
    public void addBook(CengBook book)
    {
        
        CengTreeNode temp=null;
        if(root instanceof CengTreeNodeInternal){
            temp = ((CengTreeNodeInternal)root).find_child(book);
        }
        else{
            temp = ((CengTreeNodeLeaf)root).addBook(book);
        }
        if(root!=temp){
            root=temp;
        }

    }
    public boolean is_in(Integer bookID){
        if(root instanceof CengTreeNodeInternal){
            return ((CengTreeNodeInternal)root).is_in(bookID);
        }
        else{
            return ((CengTreeNodeLeaf)root).is_in(bookID);
        }
    }
    public ArrayList<CengTreeNode> searchBook(Integer bookID)
    {
        // TODO: Search within whole Tree, return visited nodes.
        // Return null if not found.
        if(is_in(bookID)){
            ArrayList<CengTreeNode> visited = new ArrayList<CengTreeNode>();
            if(root instanceof CengTreeNodeInternal){
                visited = ((CengTreeNodeInternal)root).searchBook(bookID, visited);
            }
            else{
                visited = ((CengTreeNodeLeaf)root).searchBook(bookID, visited);
            }
            for(int i = 0; i < visited.size(); i++){
                if(visited.get(i) instanceof CengTreeNodeInternal){
                    ((CengTreeNodeInternal)visited.get(i)).printSearch(i);
                }
                else{
                    ((CengTreeNodeLeaf)visited.get(i)).printSearch(i, bookID);
                }
            }
            return visited;
        }
        else{
            System.out.println("Could not find "+Integer.toString(bookID)+".");
            return null;
        }
        
    }
    

    public void printTree()
    {
        // TODO: Print the whole tree to console
        if(root instanceof CengTreeNodeInternal){
            ((CengTreeNodeInternal)root).print(0);
        }
        else{
            ((CengTreeNodeLeaf)root).print(0);
        }
    }
    

    // Any extra functions...
}
