//import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class clean {
    public static void main(String[] args) throws InterruptedException{
        /*
        LazyList test = new LazyList();

        for (int i = 0; i < 125000; i++) {
            test.add(i);
            if(test.contains(i)) test.remove(i);
            if (i % 10000 == 0) System.out.println("Added " + i);
        }

        System.out.println("Finished adding");

        for (int i = 0; i < 125000; i++) {
            if(test.contains(i)) System.out.println("Found " + i);
        }

        for (int i = 0; i < 50000; i++) {
            test.remove(i);

            if (i % 10000 == 0) System.out.println("Removed " + i);
        }

        for (int i = 0; i < 50000; i++) {
            if(!test.contains(i)) System.out.println("Found " + i);
        }

        System.out.println("Finished");*/
        
        Thread servants[] = new Thread[4];
        for (int i = 0; i < 4; i++) {
            servants[i] = new Thread(new Servant(i*125000, (i+1)*125000, i));
        }

        long startTime = System.nanoTime();
        for (int i = 0; i < 4; i++) {
            servants[i].start();
        }
        for (int i = 0; i < 4; i++) {
            servants[i].join();
        }
        long endTime = System.nanoTime();
        long duration = (endTime - startTime);

        System.out.println(String.format("Servants took %d milliseconds to go through all 500,000 gifts.", duration/1000000));

        //System.out.println("Finished");*/
    }
}

class Node extends ReentrantLock{
    Integer item;
    boolean marked;
    Node next;

    Node(Integer item) {
        this.item = item;
    }
}

class LazyList {
    private Node head;
    private static LazyList instance = null;

    private LazyList() {
        this.head = new Node(Integer.MIN_VALUE);
        this.head.next = new Node(Integer.MAX_VALUE);
    }

    public static LazyList getInstance() {
        if (instance == null)
            instance = new LazyList();
        
        return instance;
    }

    private boolean validate(Node pred, Node curr) {
        return !pred.marked && !curr.marked && pred.next == curr;
    }

    public boolean add(Integer item) {
        while (true) {
            Node pred = head;
            Node curr = head.next;
            while (curr.item < item) {
                pred = curr; curr = curr.next;
            }
            pred.lock();
            try {
                curr.lock();
                try {
                    if (validate(pred, curr)) {
                        if (curr.item == item) {
                            //System.err.println(String.format("Failed to add %d to the List", item));
                            return false;
                        } else {
                            Node node = new Node(item);
                            node.next = curr;
                            pred.next = node;
                            //System.out.println(String.format("Succeeded in adding %d to the List", item));
                            return true;
                        }
                    }
                } finally {
                    curr.unlock();
                }
            } finally {
                pred.unlock();
            }
        }
    }

    public boolean remove(Integer item) {
        while (true) {
            Node pred = head;
            Node curr = head.next;
            while (curr.item < item) {
                pred = curr; curr = curr.next;
            }
            pred.lock();
            try {
                curr.lock();
                try {
                    if (validate(pred, curr)) {
                        if (curr.item != item) {
                            System.err.println(String.format("Failed to remove %d from the List", item));
                            return false;
                        } else {
                            curr.marked = true;
                            pred.next = curr.next;
                            //System.out.println(String.format("Succeeded in removing %d from the List", item));
                            return true;
                        }
                    }
                } finally {
                    curr.unlock();
                }
            } finally {
                pred.unlock();
            }
        }
    }

    public boolean contains(Integer item) {
        Node curr = head;
        while (curr.item < item)
            curr = curr.next;
        return curr.item == item && !curr.marked;
    }
}

class Servant extends Thread {
    int num;
    Integer low, high;
    LazyList list;

    Servant(Integer low, Integer high, int num) {
        this.low = low;
        this.high = high;
        this.list = LazyList.getInstance();
        this.num = num;
    }

    public void run() {
        System.out.println("Servant #" + this.num + " has started.");
        for (int i = low; i < high; i++) {
            this.addPresent(i);
            if (this.checkForGift(i)) this.writeThankYou(i);
            if (i%25000 == 0) 
                System.out.println("Servant #" + this.num + " has done " + (i-low) + " presents.");
        }
        System.out.println("Servant #" + this.num + " has finished.");
    }

    public void addPresent(Integer item) {
        this.list.add(item);
    }

    public void writeThankYou(Integer item) {
        this.list.remove(item);
    }

    public boolean checkForGift(Integer item) {
        return this.list.contains(item);
    }
}