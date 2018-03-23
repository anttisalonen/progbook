class Split {
    public static void main(String[] args) {
        String str = "This is a string.\n";
        String[] words = str.split(" ");
        for(String s : words) {
            System.out.println(s);
        }
    }
}

