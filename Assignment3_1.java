import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Assignment3_1 {
    public static void main(String[] args) throws InterruptedException{
        
        FineList test = new FineList();
        for (int i = 0; i < 50000; i++) {
            test.add(i);
            if(test.contains(i)) test.remove(i);
            if (i % 10000 == 0) System.out.println("Added " + i);
        }

        System.out.println("Finished adding");

        for (int i = 0; i < 50000; i++) {
            if(test.contains(i)) System.out.println("Found " + i);
        }

        for (int i = 0; i < 50000; i++) {
            test.remove(i);

            if (i % 10000 == 0) System.out.println("Removed " + i);
        }

        for (int i = 0; i < 50000; i++) {
            if(!test.contains(i)) System.out.println("Found " + i);
        }

        System.out.println("Finished");/**/
        
        /*Thread servants[] = new Thread[4];
        for (int i = 0; i < 4; i++) {
            servants[i] = new Thread(new Servant(i*25000, (i+1)*25000, i));
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

        System.out.println(String.format("Servants took %d seconds to go through all 500,000 gifts.", duration/1000000000));
        //System.out.println("Finished");*/
    }
}

class Node {
    Integer item, key;
    Node next;

    Node(Integer item) {
        this.item = item;
        this.key = item.hashCode();
    }
}

class FineNode {
    Integer item, key;
    Lock lock = new ReentrantLock();
    FineNode next;

    FineNode(Integer item) {
        this.item = item;
        this.key = item.hashCode();
    }
}

class FineList {
    private FineNode head;
    private static FineList instance = null;

    FineList() {
        this.head = new FineNode(Integer.MIN_VALUE);
        this.head.next = new FineNode(Integer.MAX_VALUE);
    }

    public static FineList getInstance() {
        if (instance == null)
            instance = new FineList();
        
        return instance;
    }

    boolean add (Integer item) {
        Integer key = item.hashCode();
        this.head.lock.lock();
        FineNode pred = head;
        try {
            FineNode curr = pred.next;
            curr.lock.lock();
            try {
                while (curr.key < key) {
                    pred.lock.unlock();
                    pred = curr;
                    curr = curr.next;
                    curr.lock.lock();
                }
                if (curr.key == key) {
                    System.err.println(String.format("Failed to add %d to the List", item));
                    return false;
                }
                FineNode newNode = new FineNode(item);
                newNode.next = curr;
                pred.next = newNode;
                //System.out.println(String.format("Succeeded in adding %d to the List", item));
                return true;
            } finally {
                curr.lock.unlock();
            }
        } finally {
            pred.lock.unlock();
        }
    }

    boolean remove (Integer item) {
        FineNode pred = null, curr = null;
        Integer key = item.hashCode();
        head.lock.lock();
        try {
            pred = head;
            curr = pred.next;
            curr.lock.lock();
            try {
                while (curr.key < key) {
                    pred.lock.unlock();
                    pred = curr;
                    curr = curr.next;
                    curr.lock.lock();
                }
                if (curr.key == key) {
                    pred.next = curr.next;
                    //System.out.println(String.format("Succeeded in removing %d from the List", item));
                    return true;
                }
                System.err.println(String.format("Failed to remove %d from the List", item));
                return false;
            } finally {
                curr.lock.unlock();
            }
        } finally {
            pred.lock.unlock();
        }
    }

    boolean contains (Integer item) {
        FineNode pred = null, curr = null;
        Integer key = item.hashCode();
        head.lock.lock();
        try {
            pred = head;
            curr = pred.next;
            curr.lock.lock();
            try {
                while (curr.key < key) {
                    pred.lock.unlock();
                    pred = curr;
                    curr = curr.next;
                    curr.lock.lock();
                }
                if (curr.key == key) {
                    //System.out.println(String.format("Succeeded in finding %d in the List", item));
                    return true;
                }
                System.err.println(String.format("Failed to find %d in the List", item));
                return false;
            } finally {
                curr.lock.unlock();
            }
        } finally {
            pred.lock.unlock();
        }
    }
}

class CoarseList {
    private Node head;
    private Lock lock = new ReentrantLock();
    private static CoarseList instance = null;

    private CoarseList() {
        this.head = new Node(Integer.MIN_VALUE);
        this.head.next = new Node(Integer.MAX_VALUE);
    }

    public static CoarseList getInstance() {
        if (instance == null)
            instance = new CoarseList();
        
        return instance;
    }

    boolean add(Integer item) {
        Node pred, curr;
        Integer key = item.hashCode();
        lock.lock();
        try {
            pred = head;
            curr = head.next;
            while (curr.key < key) {
                pred = curr;
                curr = curr.next;
            }
            if (key == curr.key) {
                System.err.println(String.format("Failed to add %d to the List", item));
                return false;
            } else {
                Node node = new Node(item);
                node.next = curr;
                pred.next = node;
                //System.out.println(String.format("Succeeded in adding %d to the List", item));
                return true;
            }
        } finally {
            lock.unlock();
        }
    }

    boolean remove(Integer item) {
        Node pred, curr;
        Integer key = item.hashCode();
        lock.lock();
        try {
            pred = head;
            curr = pred.next;
            while (curr.key < key) {
                pred = curr;
                curr = curr.next;
            }
            if (key == curr.key) {
                pred.next = curr.next;
                //System.out.println(String.format("Succeeded in removing %d from the List", item));
                return true;
            } else {
                System.err.println(String.format("Failed to remove %d from the List", item));
                return false;
            }
        } finally {
            lock.unlock();
        }
    }

    boolean contains(Integer item) {
        Node pred, curr;
        Integer key = item.hashCode();
        lock.lock();
        try {
            pred = head;
            curr = pred.next;
            while (curr.key < key) {
                pred = curr;
                curr = curr.next;
            }
            if (key == curr.key) {
                //System.out.println(String.format("Succeeded in finding %d in the List", item));
                return true;
            } else {
                System.err.println(String.format("Failed to find %d in the List", item));
                return false;
            }
        } finally {
            lock.unlock();
        }
    }
}

class Servant extends Thread {
    int num;
    Integer low, high;
    FineList list;

    Servant(Integer low, Integer high, int num) {
        this.low = low;
        this.high = high;
        this.list = FineList.getInstance();
        this.num = num;
    }

    public void run() {
        System.out.println("Servant #" + this.num + " has started.");
        for (int i = low; i < high; i++) {
            addPresent(i);
            //writeThankYou(i);
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

    public void checkForGift(Integer item) {
        this.list.contains(item);
    }
}