import java.util.Arrays;

class Filt {
	public static void main(String[] args) {
		int[] d = {3, -1, 4, -2, 5, -3, 6, -4};
		int[] d2 = filt(d);
		for(int v : d2) {
			System.out.println(v);
		}
	}

	public static int[] filt(int[] arr) {
		return Arrays.stream(arr).filter(x -> x < 0).toArray();
	}
}
