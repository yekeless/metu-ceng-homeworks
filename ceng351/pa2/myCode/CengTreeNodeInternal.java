import java.util.ArrayList;

public class CengTreeNodeInternal extends CengTreeNode
{
    private ArrayList<Integer> keys;
    private ArrayList<CengTreeNode> children;

    public CengTreeNodeInternal(CengTreeNode parent)
    {
        super(parent);
        keys = new ArrayList<Integer>();
        children = new ArrayList<CengTreeNode>();
        type = CengNodeType.Internal;

        // TODO: Extra initializations, if necessary.
    }

    public int addKey(int key)
    {
        for(int i = 0; i < keys.size(); i++){
            if(keys.get(i) > key){
                keys.add(i, key);
                return i;
            }
        }
        keys.add(keys.size(), key);
        return keys.size()-1;
    }
    public void addChildren(int index, CengTreeNode child){
        children.add(index+1, child);
    }
    public boolean checkOverflow(){
        if( keys.size() > 2 * order ){
            return true;
        }
        return false;
    }
    public void setKeys(ArrayList<Integer> new_keys){
        keys=new_keys;
    }
    public void setChildren(ArrayList<CengTreeNode> new_children){
        children=new_children;
    }
    
    public void splitInternal() {
        CengTreeNodeInternal newNode = new CengTreeNodeInternal(this.getParent());
        ArrayList<Integer> temp_key = (new ArrayList<Integer>(keys.subList(order + 1, keys.size())));
        ArrayList<CengTreeNode> temp_child = (new ArrayList<CengTreeNode>(children.subList(order + 1, children.size())));
        for(int i = 0; i < temp_child.size(); i++){
             temp_child.get(i).setParent(newNode);
        }
        newNode.setChildren(temp_child);
        newNode.setKeys(temp_key);
        int push=keys.get(order);
        keys.subList(order, keys.size()).clear();
        children.subList(order + 1, children.size()).clear();
        if (this.getParent() == null) {
            CengTreeNodeInternal newRoot = new CengTreeNodeInternal(null);
            ArrayList<Integer> new_key=new ArrayList<Integer>();
            ArrayList<CengTreeNode> new_child=new ArrayList<CengTreeNode>();
            new_key.add(push);
            new_child.add(this);
            new_child.add(newNode);
            newRoot.setKeys(new_key);
            newRoot.setChildren(new_child);
            this.setParent(newRoot);
            newNode.setParent(newRoot);
            
        } 
        else {
            CengTreeNodeInternal parent=(CengTreeNodeInternal) this.getParent();
            int index = parent.addKey(push);
            parent.addChildren(index,newNode);
            if(parent.checkOverflow()){
                parent.splitInternal();
            }

        }
    }
    public CengTreeNode find_child(CengBook book){
        CengTreeNode child=null;
        for(int i = 0; i < keys.size() ; i++){
            int key = keys.get(i);
            if(book.getBookID() < key){
                child=children.get(i);
                if(child instanceof CengTreeNodeInternal){
                     return ((CengTreeNodeInternal)child).find_child(book);
                }
                else{
                     return ((CengTreeNodeLeaf)child).addBook(book);
                }
            }
        }
        child=children.get(keys.size());
        if(child instanceof CengTreeNodeInternal){
            return ((CengTreeNodeInternal)child).find_child(book);
        }
        else{
            return ((CengTreeNodeLeaf)child).addBook(book);
        }
    }
    public void print(int tab){
        
        String tabs="";
        for(int i = 0; i < tab ;i++){
            tabs+="\t";
        }
        System.out.println(tabs+"<index>");
        String temp="";
        for(int key : keys){
            temp=tabs+key+"";
            System.out.println(temp);
        }
        System.out.println(tabs+"</index>");
        for(int i=0 ; i < children.size() ; i++){
            if(children.get(i) instanceof CengTreeNodeLeaf){
                ((CengTreeNodeLeaf)children.get(i)).print(tab+1);
            }
            else{
                ((CengTreeNodeInternal)children.get(i)).print(tab+1);
            }
        }
        
    }
    public void printSearch(int tab){
        String tabs="";
        for(int i = 0; i < tab ;i++){
            tabs+="\t";
        }
        System.out.println(tabs+"<index>");
        String temp="";
        for(int key : keys){
            temp=tabs+key+"";
            System.out.println(temp);
        }
        System.out.println(tabs+"</index>");
        
        
    }
    public ArrayList<Integer> getKeys(){
        return keys;
    }
    public boolean is_in(Integer bookID){
        for(int i = 0; i < keys.size() ; i++){
            int key = keys.get(i);
            if(bookID < key){
                if(children.get(i) instanceof CengTreeNodeInternal){
                    return ((CengTreeNodeInternal)children.get(i)).is_in(bookID);
                }
                else{
                    return ((CengTreeNodeLeaf)children.get(i)).is_in(bookID);
                }
            }
        }
        if(children.get(keys.size()) instanceof CengTreeNodeInternal){
            return ((CengTreeNodeInternal)children.get(keys.size())).is_in(bookID);
        }
        else if(children.get(keys.size()) instanceof CengTreeNodeLeaf){
            return ((CengTreeNodeLeaf)children.get(keys.size())).is_in(bookID);
        }
        return false;
        
        
    }
    public ArrayList<CengTreeNode> searchBook(Integer bookID, ArrayList<CengTreeNode> visited)
    {
        for(int i = 0; i < keys.size() ; i++){
            int key = keys.get(i);
            if(bookID < key){
                if(children.get(i) instanceof CengTreeNodeInternal){
                    visited.add(this);
                    return ((CengTreeNodeInternal)children.get(i)).searchBook(bookID, visited);
                }
                else{
                    visited.add(this);
                    return ((CengTreeNodeLeaf)children.get(i)).searchBook(bookID, visited);
                }
            }
        }
        if(children.get(keys.size()) instanceof CengTreeNodeInternal && bookID >= keys.get(keys.size()-1)){
            visited.add(this);
            return ((CengTreeNodeInternal)children.get(keys.size())).searchBook(bookID, visited);
        }
        else if(children.get(keys.size()) instanceof CengTreeNodeLeaf && bookID >= keys.get(keys.size()-1)) {
            visited.add(this);
            return ((CengTreeNodeLeaf)children.get(keys.size())).searchBook(bookID, visited);
        }
        return visited;
    }   

    // GUI Methods - Do not modify
    public ArrayList<CengTreeNode> getAllChildren()
    {
        return this.children;
    }
    public Integer keyCount()
    {
        return this.keys.size();
    }
    public Integer keyAtIndex(Integer index)
    {
        if(index >= this.keyCount() || index < 0)
        {
            return -1;
        }
        else
        {
            return this.keys.get(index);
        }
    }

    // Extra Functions
}
