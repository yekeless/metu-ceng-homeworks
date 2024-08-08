def find_the_children(lst,string):
    childrens=[]
    a=[]
    for person in lst:
        if "CHILD" in person:
            a.append(person)
    for person in a:
        b=person.split()
        if b[1]==string or b[2]==string:
            for c in b[3:]:
                childrens.append(c)
    return childrens
def find_the_parents(lst,string):
    parents=[]
    a=[]
    for person in lst:
        if "CHILD" in person:
            a.append(person)
    for person in a:
        b=person.split()
        if string in b:
            if string!=b[1] and string!=b[2]:
                parents=[b[1],b[2]]
    return parents
def find_the_departed(lst):
    a=[]
    departed=[]
    for person in lst:
        if "DEPARTED" in person:
            a.append(person)
        elif "DECEASED" in person:
            a.append(person)
    for person in a:
        b=person.split()
        departed.append(b[1])
    return departed
def find_the_married_persons(lst):
    a=[]
    married=[]
    for person in lst:
        if "MARRIED" in person:
            a.append(person)
    for person in a:
        b=person.split()
        married.append((b[1],b[2]))
    return married
def find_the_first_PG(lst,string):
    first_PG=[]
    children=find_the_children(lst,string)
    departed=find_the_departed(lst)
    for child in children:
        if child not in departed:
            first_PG.append(child)
        elif child in departed:
            if find_the_first_PG(lst,child)!=[]:
                first_PG.append(find_the_first_PG(lst,child))

    return first_PG
def find_the_second_PG(lst,string):
    second_PG=[]
    parents=find_the_parents(lst,string)
    departed=find_the_departed(lst)
    for parent in parents:
        if parent not in departed:
            second_PG.append(parent)
        if parent in departed:
            if find_the_first_PG(lst,parent)!=[]:
                second_PG.append(find_the_first_PG(lst,parent))
    return second_PG
def find_the_third_PG(lst,string):
    third_PG=[]
    departed=find_the_departed(lst)
    parents=find_the_parents(lst,string)
    grandparents=[]
    for parent in parents:
        gp=find_the_parents(lst,parent)
        for grp in gp:
            grandparents.append(grp)
    for grandparent in grandparents:
        if grandparent not in departed:
            third_PG.append(grandparent)
        elif grandparent in departed:
            if find_the_first_PG(lst,grandparent)!=[]:
                third_PG.append(find_the_first_PG(lst,grandparent))

    return third_PG
def find_the_spouse(lst,string):
    married=find_the_married_persons(lst)
    spouse=[]
    departed=find_the_departed(lst)
    for marriage in married:
        if string==marriage[0]:
            spouse=[marriage[1]]
        elif string==marriage[1]:
            spouse=[marriage[0]]
    if spouse!=[]:
        if spouse[0] in departed:
            spouse=[]
    return spouse
def create_queue():
    return []
def enqueue(item, queue):
    queue.append(item)
def dequeue(queue):
    return queue.pop(0)
def front(queue):
    return queue[0]
def is_empty(queue):
    return queue == []
def flatten(lst):
    flat_list = []
    q =create_queue()

    for e in lst:
        enqueue(e, q)
    while(not is_empty(q)):
        first = dequeue(q)
        if type(first) == list:
            for e in first:
                enqueue(e,q)
        else:
            flat_list += [first]
    return flat_list

def property_share(inheritance_lst,property):
    shares=[]
    share=len(inheritance_lst)
    for heir in inheritance_lst:
        if type(heir)==list:
            shares.append(property_share(heir,property/share))
        elif type(heir)!=list:
            shares.append((heir,property/share))
    return flatten(shares)
def sum_same_shares(lst):
    result=[]
    for share in lst:
        string=share[0]
        property=share[1]
        result.append([string,property])
    for person in result:
        a=result.index(person)
        lst=result[a+1:]
        for property in lst:
            if person[0]==property[0]:
                property[1]=person[1]+property[1]
                del result[a]
    output=[]
    for a in result:
        name=a[0]
        mal=a[1]
        output.append((name,mal))
    return output
def inheritance(lst):
    dec_lst=[]
    output=[]
    departed=find_the_departed(lst)
    for person in lst:
        if "DECEASED" in person:
            a=person.split()
            dec_lst.append(a[1])
            dec_lst.append(a[2])
    deceased=dec_lst[0]
    property=float(dec_lst[1])

    first=find_the_first_PG(lst,deceased)
    second=find_the_second_PG(lst,deceased)
    third=find_the_third_PG(lst,deceased)
    spouse=find_the_spouse(lst,deceased)
    if spouse!=[]:
        if first!=[]:
            output=[(spouse[0],property/4)]+property_share(first,property*3/4)
        elif first==[]:
            if second!=[]:
                output=[(spouse[0],property/2)]+property_share(second,property/2)
            elif second==[]:
                if third!=[]:
                    output=[(spouse[0],property*3/4)]+property_share(third,property/4)
                elif third==[]:
                    output=[(spouse[0],property)]
    if spouse==[]:
        if first!=[]:
            output=property_share(first,property)
        elif first==[]:
            if second!=[]:
                output=property_share(second,property)
            elif second==[]:
                if third!=[]:
                    output=property_share(third,property)
    return sum_same_shares(output)

