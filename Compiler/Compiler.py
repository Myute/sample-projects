#-------------------------------------------------#
#   CS-351                                        #
#   Basic PL Tokenization and Syntactic Analysis  #
#   By: Preston Roberts                           #
#-------------------------------------------------#

# developed with the help of the TKinter documentation, and a lot of trial-and-error.

import re;
from tkinter import *;
from tkinter import font;

# establishing new "tree" class
# I didn't make it fully traversable because I didn't need it to be.
class proNode:
    def __init__(self, nodeVal=None, parent=None, depth=0):
        self.depth = depth;
        self.branches = 0;
        self.children = [];
        self.parent = parent;
        self.value = nodeVal;
        return;
    
    def addChild(self, childVal):
        if type(childVal) is not proNode:
            nChild = proNode(childVal,self);
            self.children.append(nChild);self.branches += 1;
            nChild.setParent(self);
            nChild.setDepth(self.depth + 1);
        else: 
            self.children.append(childVal);self.branches += 1;
            childVal.setParent(self);
            childVal.setDepth(self.depth + 1);
        return;

    def addChildAt(self, childVal, loc):
        if childVal:
            nChild = proNode(childVal,self);
            self.children.insert(loc, nChild);self.branches += 1;
            nChild.setParent(self);
        return;

    def getVal(self):
        return self.nodeVal;
    
    def getChildren(self):
        return self.children;

    def branches(self):
        return self.branches;

    def getDepth(self):
        return self.depth;

    def setDepth(self, depth):
        self.depth = depth;
        if self.children:
            for child in self.children:
                child.setDepth(depth + 1);
        return;

    def parentUp(self, parentNode):
        self.parent = parentNode;
        self.setDepth(parentNode.getDepth() + 1);
        parentNode.addChild(self);
        return;    
    
    def parentUpAt(self, parentNode, idx):
        self.parent = parentNode;
        self.setDepth(parentNode.getDepth() + 1);
        parentNode.addChildAt(self, idx);
        return;

    def setParent(self, parentNode):
        self.parent = parentNode;
        return;

    def returnAll(self):
        output = [];
        output.append("node at "+str(self.depth)+" \tvalue: "+str(self.value));
        
        if self.children:
            for n in self.children:
                n.valCourrier(output);

        return '\n'.join(output);

    def valCourrier(self, valr):
        if self.parent:
            valr.append("node at "+str(self.depth)+" \tvalue: "+str(self.value)+" \t\tparent: "+str(self.parent.value));
        else:
            valr.append("node at "+str(self.depth)+" \tvalue: "+str(self.value));
        
        if self.children:
            for n in self.children:
                n.valCourrier(valr);

        return;



# finished establishing new "tree" class

# logic keyword counts (for checking statement validity)

keywordCount = {
    "if" : 0,
    "else" : 0,
    "int" : 0,
    "print" : 0,
};

# regex token pattern map (dict)

searchMap = {
    ("if","keyword","conditional"): re.compile(r'((?<=[\s])|^|\b)if((?=[$,\:,\(])|\b)'),
    ("else","keyword","conditional"): re.compile(r'((?<=[\s,\b])|^|\b)else((?=[$,\:,\(])|\b)'),
    ("int","keyword","type"): re.compile(r'((?<=[\s,\b,\(,\)])|^|\b)int((?=[$,\(,\),\s,\t])|\b)'),
    ("float","keyword","type"): re.compile(r'((?<=[\s,\b,\(,\)])|^|\b)float((?=[$,\(,\),\s,\t])|\b)'),
    ("print","keyword","function"): re.compile(r'((?<=[\s,\b])|^|\b)print((?=[\(]))'),
    ("=","operator","assignment"): re.compile(r'='),
    ("+","operator","math"): re.compile(r'\+'),
    ("-","operator","math"): re.compile(r'\-'),
    (">","operator","math"): re.compile(r'>'),
    ("*","operator","math"): re.compile(r'\*'),
    ("/","operator","math"): re.compile(r'\/'),
    ("%","operator","math"): re.compile(r'\%'),
    ("(","separator","segment"): re.compile(r'\('),
    (")","separator","segment"): re.compile(r'\)'),
    (":","separator","scope"): re.compile(r':'),
    #("\"...\"","sp"): re.compile(r'\"[\s\S]*[\w\W]*[\d\D]*\"'),
    ("str","literal","string"): re.compile(r'([\"\'])(?:(?=(\\?))\2.)*?\1'), 
    #("\"","separator","segment"): re.compile(r'\"'),
    ("var","identifier","id"): re.compile(r'((?<=[\s,=,\+,\-,\/,\%,\*,\),\(,<,>,\.])|^|\b)(?<!\")[a-zA-z]+\d*(?!\")(?=\s+|$|\b|=|[\+,\-,\/,\%,\*,\),\(,<,>,\.])',re.MULTILINE), #this got a tad long, but I think this is all cases. It was easier to work inclusively than exclusively here. Also got longer because I had to negative lookahead and behind for quotation marks.
    ("integer","literal","int"): re.compile(r'((?<=[\s,=,\+,\-,\/,\%,\*,\),\(,<,>,\,])|^)\d+(?=\s+|$|\b|=|[\+,\-,\/,\%,\*,\),\(,<,>,\.])',re.MULTILINE),
    ("float","literal","float"): re.compile(r'((?<=[\s,=,\+,\-,\/,\%,\*,\),\(,<,>,\,,\.])|^|\b)\d+\.\d+(?=\s+|$|\b|=|[\+,\-,\/,\%,\*,\),\(,<,>,\.,\,])',re.MULTILINE),
    #("str","literat"): re.compile(r'\b[a-zA-z]+\b'),
}
searchView = searchMap.items();

class PrestonGUI:
    
    def __init__(self, TKmaster):
        self.kymod = dict(keywordCount);
        self.lineVal = [];
        self.outVal = [];
        self.master = TKmaster;
        self.itLine = 0; self.clines = 0;
        self.lineDisp = StringVar(self.master,"No Current Line"); 
        self.master.title("Basic Tokenization and Parsing Program"); #title
        self.master.geometry("1000x500"); #resize the canvas so I don't have to manually
        self.master["background"]="gray21";

        #top header
        self.topFrame = Frame(self.master, background="gray18");
        self.topFrame.grid_propagate(False);
        self.topFrame.grid(row=0, sticky=N+E+W+S);
        self.topLabel = Label(self.topFrame,text="Tokenization & Parsing Framework", font=font.Font(self.master,family="Georgia",size=14), foreground="white",  background="gray18");
        self.topLabel.grid(sticky=N+E+S+W, pady=8); # make toplabel stick to top of row, and give it some y-padding.
        self.topFrame.columnconfigure(0,weight=1);
        self.midFrame = Frame(self.topFrame, background="gray18"); # this way I'm taking one large auto-resizing column, and makign a grid inside of it.
        self.midFrame.grid_propagate(False); # don't need grid propagation for midframe so that scaling can be easier/more-precise.
        self.midFrame.grid(row=1, sticky=N+E+S+W);
        self.midFrame.columnconfigure(0,weight=4);#, minsize=300); #set up for the left/right frames
        self.midFrame.columnconfigure(1,weight=3);#, minsize=300);
        self.midFrame.columnconfigure(2,weight=5);
        self.topFrame.rowconfigure(1, weight=1);

        self.master.columnconfigure(0,weight=1);
        self.master.rowconfigure(0, weight=2);
        self.master.rowconfigure(1, minsize=70);# row 1 is where I'll be putting the buttons and counter.
        self.midFrame.rowconfigure(0, weight=1);# this is the primary row of the midframe, so let's stretch it. 

        # bottom row initilization
        self.nextButton = Button(self.master,background="gray20",command=self.next_line ,foreground="white",font=font.Font(self.master,family="Georgia",size=12),padx=3,text="Next Line"); self.nextButton.grid(row=1,column=0,sticky=W,padx=50);
        self.lineLabel = Label(self.master,textvariable=self.lineDisp, font=font.Font(self.master,family="Georgia",size=12), foreground="white",  background="gray21"); self.lineLabel.grid(row=1,column=0,sticky=E,padx=50);
        # self.lineDisp = "Current Line: 0";
        # self.master.bind('<>', self.next_line); # this was going to be for proceeding to next line. I ended up just making it a gui button instead so text input would be more straightforward.

        self.leftFrame = Frame(self.midFrame, background="gray10");  self.leftFrame.grid_propagate(False); self.leftFrame.grid(row=0, column=0, sticky=N+E+S+W);
        self.rightFrame = Frame(self.midFrame, background="gray10"); self.rightFrame.grid_propagate(False); self.rightFrame.grid(row=0, column=2, sticky=N+E+S+W);
        self.centerFrame = Frame(self.midFrame, background="gray10"); self.centerFrame.grid_propagate(False); self.centerFrame.grid(row=0, column=1, sticky=N+E+S+W);
        self.leftFrame.columnconfigure(1,weight=1);
        self.leftFrame.rowconfigure(1,weight=1);
        self.centerFrame.columnconfigure(1,weight=1);
        self.centerFrame.rowconfigure(1,weight=1);
        self.rightFrame.columnconfigure(1,weight=1);
        self.rightFrame.rowconfigure(1,weight=1);
        # breaks up the envorinment body into topFrame and midFrame that can scale individually.

        # the labels that I alomst forgot to add to the input and output boxes.
        self.inputLabel = Label(self.leftFrame, text="Code Input", font=font.Font(self.master,family="Georgia",size=12), foreground="white",  background="gray10"); self.inputLabel.grid(row=0, column=1, sticky=S, pady=(5,0));
        self.outputLabel = Label(self.centerFrame, text="Token Output", font=font.Font(self.master,family="Georgia",size=12), foreground="white",  background="gray10"); self.outputLabel.grid(row=0, column=1, sticky=S, pady=(5,0));
        self.treeLabel = Label(self.rightFrame, text="Parse Tree", font=font.Font(self.master,family="Georgia",size=12), foreground="white",  background="gray10"); self.treeLabel.grid(row=0, column=1, sticky=S, pady=(5,0));

        # input, output, and line number boxes....
        self.intpuText = Text(self.leftFrame, undo=True, maxundo=-1, state="normal", wrap="none", background="gray10", borderwidth="1", foreground="white",highlightcolor="white", insertbackground="white", font=font.Font(self.master,family="Courier",size=12));self.intpuText.grid(row=1,column=1,sticky=N+E+S+W, padx="16", pady=(5,12));
        self.inputLine = Text(self.leftFrame, background="gray11", state="disabled", foreground="white", width="4", font=font.Font(self.master,family="Courier",size=12), borderwidth=0); self.inputLine.grid(row=1,column=0,sticky=N+E+S+W, pady=(5,12)); self.leftFrame.columnconfigure(0,minsize=30);
        self.inputLine.tag_configure("Right",justify='right'); #need to add this tag to all line numbering text because it's the only way to justify right with the text widget.
        self.outpuText = Text(self.centerFrame, undo=True, state="disabled", wrap="none", background="gray10", borderwidth="1", foreground="white",highlightcolor="white", insertbackground="white", font=font.Font(self.master,family="Courier",size=12));self.outpuText.grid(row=1,column=1,sticky=N+E+S+W, padx="12", pady=(5,12));
        # tree canvas
        self.treeOut = Text(self.rightFrame, state="disabled", wrap="none", background="gray10", borderwidth="1", foreground="white",highlightcolor="white", insertbackground="white", font=font.Font(self.master,family="Courier",size=12)); self.treeOut.grid(row=1, column=1, sticky=N+E+S+W, padx=(5,20), pady=(5,12));
        # I wanted to make the inputLine (count) a label instead of a text box, but unfortunately you can't scroll a label like I would want.
        self.intpuText.tag_configure("High", background="coral");
        self.inputLine.config(state="normal");
        self.inputLine.insert(INSERT,"0", "Right"); # adding zero (which will never be deleted so we dont' have leading new lines) and justifying right.
        self.inputLine.config(state="disabled");

        # scrollbar
        self.uniscrollbar = Scrollbar(self.leftFrame, activerelief=None);self.uniscrollbar.grid(row=1, column=2, sticky=NS, pady=(5,12));
        self.uniscrollbar.config(command=self.scrollBoth);
        self.intpuText.config(yscrollcommand=self.updateScroll);
        self.inputLine.config(yscrollcommand=self.updateScroll);

        self.resultscrollbar = Scrollbar(self.centerFrame, activerelief=None);self.resultscrollbar.grid(row=1, column=2, sticky=NS, pady=(5,12), padx=(0,12));
        self.resultscrollbar.config(command=self.scrollResult);
        self.outpuText.config(yscrollcommand=self.updateScrollResult);

        # menu creation
        self.topMenu = Menu(self.master);
        self.topMenu.add_command(label="Quit",command=self.master.quit);
        self.topMenu.add_command(label="New",command=self.__newGUI__);
        self.master.configure(menu=self.topMenu);
        
        # ease of access
        self.master.bind("<Escape>", quit); # hitting escape will end the run
        self.master.bind("<Return>", self.checkInputLines);
        self.master.bind("<Control-v>", self.DelCheckInput); # added this to cover the "paste from clipboard" event which could add lines
        self.master.bind("<BackSpace>", self.DelCheckInput); # this could be commented out if more performance is required (triggers update on every backspace)


    # next line iterator
    def next_line(self, event=None):
        if((self.clines == 0 and self.itLine == 0) or (self.clines != 0 and self.clines > self.itLine)):
            self.intpuText.tag_remove("High","1.0",END);
            self.lineVal.insert( self.itLine, str(self.intpuText.get(str(self.itLine + 1)+".0", str(self.itLine + 2)+".0")) );
            self.intpuText.tag_add("High",str(self.itLine + 1)+".0",str(self.itLine + 2)+".0");
            self.treeOut.config(state="normal");
            self.treeOut.delete("1.0",END);
            self.treeOut.config(state="disabled");
            self.lineDisp.set("Current Line: "+str(self.itLine));
            self.line_parse(self.lineVal,self.itLine,self.kymod);
            self.itLine+=1;
        elif(self.itLine >= self.clines):
            self.intpuText.tag_remove("High","1.0",END);
            self.itLine = 0;
            self.outpuText.config(state="normal");
            self.outpuText.delete("1.0",END);
            self.outpuText.config(state="disabled");
            self.lineDisp.set("No Current Line");
            self.treeOut.config(state="normal");
            self.treeOut.delete("1.0",END);
            self.treeOut.config(state="disabled");
            self.kymod = dict(keywordCount);

        return;

    def sortNum(self,tup): # for sorting function below
        return tup[0];

    # separate parsing function
    def line_parse(self, incoming, idx, keywords):

        self.treeOut.delete("all");
        resultSet = [];
        work = "";
        self.treeOut.delete("all");
        for token in searchView:
            res = token[1].finditer(incoming[idx]);
            for m in res:
                try: # avoid performance drops by using try instead of relying on branch prediction
                    strnky = m.string[m.start():m.end()];
                    # print(strnky);
                    work += keywords.get(token[0][0],"& ");
                    work += keywords.get(strnky,"| ");
                    resultSet.insert(m.start(),(m.start(), token[0][1], token[0][0], m.string[m.start():m.end()], token[0][2]));
                    pass;
                    # I'll admit, this is probably a rather convoluted solution, but it will be better for longer scripts, because the only tokens that will take longer will be keywords.
                    # and this way I'll be collecting keyword statistics to make sure that every else has an if, etc.
                except:
                    if token[0][1] == "keyword":
                        resultSet.insert(m.start(),(m.start(), token[0][1], token[0][0], m.string[m.start():m.end()], token[0][2]));
                        # token = (start(int), type(in token pair), name/val, actual value, token-discernable type (Ex: math))
                        keywords[token[0][0]] += 1;
                        # print(keywords); # test output (ignore)
                        pass;
                    else:
                        pass;


        resultSet.sort(key=self.sortNum);
        
        self.outpuText.config(state="normal");
        self.outpuText.insert(INSERT,"\nline: "+str(idx)+"\n");
        self.outpuText.config(state="disabled");

        # display the tokens as they are read in from the results of the text input (highlighted) line
        for res in resultSet:
            self.outpuText.config(state="normal"); # allow writing to the box
            self.outpuText.insert(INSERT,"<"+res[1]+","+ res[3].replace('\n',' ').replace('\r',' ') +">\n"); # write the token indentified
            self.outpuText.config(state="disabled"); # disable writing to the box again (a common loop), probably could have made it a function, but this way I can pass other prarmeters as well if need be
        
        self.master.update();

        print(resultSet); # testing output (ignore)

        root = self.parseContent(resultSet); #what I didn't realize is that if there is a two-value tuple, and one of the values is "None" it will look like it is only the one value, and not a "tuple" type...
        if type(root) is not proNode: #this is to solve that above comment.
            root = root[0];
        

        if root: # output the given parse tree information (with tree nodes numbered left to right)
            self.treeOut.config(state="normal");
            self.treeOut.insert(INSERT, root.returnAll()); # display all children starting at the tree root (after root confirmation above) may change after parsing
            self.treeOut.config(state="disabled")

        return;

    #I found it was easier to make a semi-recursive state machine over the example script given to us.
    def parseContent(self, cArray):
        ary = list(cArray);
        tNode = None;
        cNode = None;
        while ary:
            tkn = ary.pop(0);

            if tkn[4] == "type":
                if ary[0][4] == "id":
                    iden = ary.pop(0);
                    cNode = proNode(tkn[3].replace('\n',' ').replace('\r',' '));
                    ctype = proNode("id");
                    ctype.addChild(proNode("\""+iden[3].replace('\n',' ').replace('\r',' ')+"\""));
                    cNode.addChild(ctype);
            
            elif tkn[4] == "assignment":
                prevNode = cNode;
                cNode = proNode("exp");
                cNode.addChild(prevNode);
                cNode.addChild("=");
                nex = self.parseContent(ary);
                ary = nex[1];
                cNode.addChild(nex[0]);
                return cNode;

            elif tkn[2] == "if":
                cNode = proNode("if exp");
                cSub = ary.pop(0);
                if cSub[2] == "(":
                    expAry = [];
                    while cSub[2] != ")" and ary:
                        cSub = ary.pop(0);
                        expAry.append(cSub[3].replace('\n',' ').replace('\r',' '));
                    cSub = ary.pop(0);
                    if cSub[2] == ":":
                        cExp = proNode(str(" ".join(expAry)));
                        cNode.addChild(cExp);
                        cTh = proNode("then");
                        cTh.addChild("PC iterate"); #just to flesh it out with what we learned in 331
                        cEl = proNode("else");
                        cEl.addChild("PC jump");
                        cExp.addChild(cTh);
                        cExp.addChild(cEl); #I'm just going to be honest, the BNF grammar in HW5 for the "if" statement was a little vague.

            elif tkn[2] == "print":
                cNode = proNode("print func");
                cSub = ary.pop(0);
                if cSub[2] == "(":
                    cPram = ary.pop(0);
                    cSub = ary.pop(0);
                    if cSub[2] == ")":
                        #print("print working"); # test output (ignore)
                        if cPram[4] == "string" or cPram[4] == "float" or cPram[4] == "int" or cPram[4] == "id": #this isn't my best if statement. There's a better way to do this.
                            cTyp = proNode(cPram[4]);
                            cTyp.addChild(cPram[3].replace('\n',' ').replace('\r',' '));
                            cNode.addChild(cTyp);
                        

            elif tkn[1] == "literal" and tkn[4] != "string":
                if len(ary) > 1 and ary[0][4] == "math":
                    #print(ary);
                    if len(ary) == 2 or ary[2][4] != "math":
                        cMath = ary.pop(0);
                        if cMath[2] == "*":
                            cNode = proNode("multi");
                        else:
                            cNode = proNode("math");
                        lit = proNode(tkn[4]);
                        lit.addChild(proNode(tkn[3].replace('\n',' ').replace('\r',' ')));
                        cNode.addChild(lit);
                        cNode.addChild(proNode(cMath[3].replace('\n',' ').replace('\r',' ')));
                        cLit = ary.pop(0);
                        if cMath[2] == "*":
                            lit2Node = proNode("multi");
                        else:
                            lit2Node = proNode("math");
                        lit2 = proNode(cLit[4]);
                        lit2.addChild(proNode(cLit[3].replace('\n',' ').replace('\r',' ')));
                        lit2Node.addChild(lit2);
                        cNode.addChild(lit2Node);
                    else:
                        cMath = ary.pop(0);
                        if ary[0][2] == "*":
                            cNode = proNode("multi");
                        else:
                            cNode = proNode("math");
                        lit = proNode(tkn[4]);
                        lit.addChild(proNode(tkn[3].replace('\n',' ').replace('\r',' ')));
                        cNode.addChild(lit);
                        cNode.addChild(proNode(cMath[3].replace('\n',' ').replace('\r',' ')));
                        nex = self.parseContent(ary);
                        ary = nex[1];
                        cNode.addChild(nex[0]); 
        
            return(cNode, ary);



    # the event-based number updating for the input box
    def checkInputLines(self, event=None):
        self.clines+=1; # line count (total)
        self.gotLines = int(self.intpuText.index('end').split('.')[0]) - 1; # get number of existing lines
        #print("iter: "+str(self.gotLines)+" : "+str(self.clines)+"\n"); # test output (ignore)
        if(self.gotLines != self.clines): # if the lines number that we've just called is different from our count (someone deleted some lines) then redraw
            #print("redrawn"); # functionality test (ignore)
            self.clines = self.gotLines; # set the line count to whatever the actual number has changed to.
            self.inputLine.config(state="normal");
            self.inputLine.delete('2.0', END);
            for line in range(1,(self.gotLines)):
                self.inputLine.insert(INSERT,"\n"+str(line), "Right");
            self.inputLine.config(state="disabled");
        else:
            #print("iterated"); # functionality test (ignore)
            self.inputLine.config(state="normal");
            self.inputLine.insert(INSERT,"\n"+(str(self.clines - 1)), "Right");
            self.inputLine.config(state="disabled");
        return;

    # this was more of a detail issue for me, but I didn't like how infrequently it was redrawing the line numbers, so I made a different event for backspace
    def DelCheckInput(self,event=None):
        
        self.delLines = int(self.intpuText.index('end').split('.')[0]) - 2; # minus 2 instead of 1 because we want one less line (-1) and we're starting at 0 not 1 (so -1 again).
        #print("del: "+str(self.delLines)+" : "+str(self.clines)+"\n"); # functionality test (ignore)
        if(self.clines != self.delLines):
            self.checkInputLines();
        return;

    # couldn't really find a better way to contol the scrolling
    def scrollResult(self, action, position, type=None):
        self.outpuText.yview_moveto(position);

    def scrollBoth(self, action, position, type=None):
        self.intpuText.yview_moveto(position);
        self.inputLine.yview_moveto(position);

    def updateScroll(self, first, last, type=None):
        self.intpuText.yview_moveto(first);
        self.inputLine.yview_moveto(first);
        self.uniscrollbar.set(first, last);

    def updateScrollResult(self, first, last, type=None):
        self.outpuText.yview_moveto(first);
        self.resultscrollbar.set(first,last);

    # for the "new" button
    def __newGUI__(self):
        PrestonGUI(Tk());
    

    
root = Tk();
my_gui = PrestonGUI(root);
root.mainloop();